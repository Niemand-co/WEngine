#include "pch.h"
#include "Render/Public/RenderDependencyGraphParameter.h"

void GetPassAccess(EPassFlag Flag, EAccess& SRVAccess, EAccess& UAVAccess)
{
	if (WEngine::EnumHasFlags(Flag, EPassFlag::Raster))
	{
		SRVAccess |= EAccess::SRVGraphics;
		UAVAccess |= EAccess::UAVGraphics;
	}

	if (WEngine::EnumHasFlags(Flag, EPassFlag::Compute) || WEngine::EnumHasFlags(Flag, EPassFlag::AsyncCompute))
	{
		SRVAccess |= EAccess::SRVCompute;
		SRVAccess |= EAccess::UAVCompute;
	}

	if (WEngine::EnumHasFlags(Flag, EPassFlag::Copy))
	{
		SRVAccess |= EAccess::CopySrc;
	}
}

void WShaderParameterMetaData::InitializeLayout()
{
	if(Members.Size() == 0)
		return;

	auto IsRDGTextureResource = [](EUniformBaseType Type)->bool{ return Type == EUniformBaseType::UB_RDG_TEXTURE     ||
																		Type == EUniformBaseType::UB_RDG_TEXTURE_SRV ||
																		Type == EUniformBaseType::UB_RDG_TEXTURE_UAV ;};

	auto IsRDGBufferResource = [](EUniformBaseType Type)->bool { return Type == EUniformBaseType::UB_RDG_BUFFER_SRV  ||
																		Type == EUniformBaseType::UB_RDG_BUFFER_SRV ;};

	for (int32 Index = Members.Size() - 1; Index >= 0; --Index)
	{
		WParameterMember& Member = Members[Index];
		if (IsRDGTextureResource(Member.BaseType))
		{
			Layout.GraphResources.Push(ShaderParametersLayout::ResourceInfo(Member.Offset, Member.BaseType));
			Layout.GraphTextures.Push(ShaderParametersLayout::ResourceInfo(Member.Offset, Member.BaseType));

		}
		else if (Member.BaseType == EUniformBaseType::UB_RTV)
		{
			Layout.RenderTargetOffset = Member.Offset;
			Layout.GraphResources.Push(ShaderParametersLayout::ResourceInfo(Member.Offset, Member.BaseType));
			Layout.GraphTextures.Push(ShaderParametersLayout::ResourceInfo(Member.Offset, Member.BaseType));
		}
		else if (IsRDGBufferResource(Member.BaseType))
		{
			Layout.GraphResources.Push(ShaderParametersLayout::ResourceInfo(Member.Offset, Member.BaseType));
			Layout.GraphBuffers.Push(ShaderParametersLayout::ResourceInfo(Member.Offset, Member.BaseType));
			Layout.GraphBuffers.Push(ShaderParametersLayout::ResourceInfo(Member.Offset, Member.BaseType));
		}

	}

	Layout.ConstantBufferSize = Size;
}

RHIRenderPassDescriptor WRDGParameterStruct::GetRenderPassInfo() const
{
	RHIRenderPassDescriptor descriptor = {};
	const WRDGRenderTargetBindingSlots& RenderTargets = GetRenderTarget();
	
	uint32 ColorAttachmentIndex = 0;
	RenderTargets.Enumerate([&descriptor, &ColorAttachmentIndex](const WRDGRenderTargetBinding& RenderTarget)
	{
		WRDGTexture *Texture = RenderTarget.Texture;
		AttachmentStoreOP StoreOp = WEngine::EnumHasFlags(Texture->Desc.Flags, ETextureCreateFlags::TextureCreate_CPUReadable) ? AttachmentStoreOP::DontCare : AttachmentStoreOP::Store;
		AttachmentLoadOP LoadOp = RenderTarget.LoadOP;

		{
			descriptor.ColorAttachmentDescriptors[ColorAttachmentIndex].attachmentFormat = Texture->Desc.Format;
			descriptor.ColorAttachmentDescriptors[ColorAttachmentIndex].attachmentLoadOP = LoadOp;
			descriptor.ColorAttachmentDescriptors[ColorAttachmentIndex].attachmentStoreOP = StoreOp;
			descriptor.ColorAttachmentDescriptors[ColorAttachmentIndex].sampleCount = Texture->Desc.sampleCount;
		}

		++ColorAttachmentIndex;
	});
	descriptor.ColorAttachmentCount = ColorAttachmentIndex;

	const WRDGDepthStencilBinding& DepthStencil = RenderTargets.DepthStencilTexture;
	if (DepthStencil.Texture)
	{
		WRDGTexture *Texture = DepthStencil.Texture;
		AttachmentStoreOP DepthStoreOp = Texture->Desc.IsDepthFormat() ? AttachmentStoreOP::Store : AttachmentStoreOP::DontCare;
		AttachmentStoreOP StencilStoreOp = Texture->Desc.IsStencilFormat() ? AttachmentStoreOP::Store : AttachmentStoreOP::DontCare;
		AttachmentLoadOP DepthLoadOp = DepthStencil.DepthLoadOP;
		AttachmentLoadOP StencilLoadOp = DepthStencil.StencilLoadOP;

		descriptor.DepthStencilAttachmentDescriptor.attachmentFormat = Texture->Desc.Format;
		descriptor.DepthStencilAttachmentDescriptor.attachmentLoadOP = DepthLoadOp;
		descriptor.DepthStencilAttachmentDescriptor.attachmentStoreOP = DepthStoreOp;
		descriptor.DepthStencilAttachmentDescriptor.stencilLoadOP = StencilLoadOp;
		descriptor.DepthStencilAttachmentDescriptor.stencilStoreOP = StencilStoreOp;
		descriptor.DepthStencilAttachmentDescriptor.sampleCount = Texture->Desc.sampleCount;

		descriptor.bHasDepthStencilAttachment = true;
	}

	return descriptor;
}

RHIFramebufferDescriptor WRDGParameterStruct::GetFramebufferInfo() const
{
	RHIFramebufferDescriptor descriptor = {};
	const WRDGRenderTargetBindingSlots& RenderTargets = GetRenderTarget();

	uint32 NumColorAttachment = 0;
	RenderTargets.Enumerate([&descriptor, &NumColorAttachment](const WRDGRenderTargetBinding& RenderTarget)
	{
		WRDGTexture *Texture = RenderTarget.Texture;
		descriptor.Attachments[NumColorAttachment++] = (WTextureRHIRef)Texture->GetRHI();
		descriptor.extent.width = Texture->Desc.Extent.width < descriptor.extent.width ? Texture->Desc.Extent.width : descriptor.extent.width;
		descriptor.extent.height = Texture->Desc.Extent.height < descriptor.extent.height ? Texture->Desc.Extent.height : descriptor.extent.height;
		descriptor.extent.depth = Texture->Desc.Extent.depth < descriptor.extent.depth ? Texture->Desc.Extent.depth : descriptor.extent.depth;
	});

	WRDGTexture *DepthStencil = RenderTargets.DepthStencilTexture.Texture;
	if (DepthStencil)
	{
		descriptor.Attachments[NumColorAttachment++] = (WTextureRHIRef)DepthStencil->GetRHI();
		descriptor.extent.width = DepthStencil->Desc.Extent.width < descriptor.extent.width ? DepthStencil->Desc.Extent.width : descriptor.extent.width;
		descriptor.extent.height = DepthStencil->Desc.Extent.height < descriptor.extent.height ? DepthStencil->Desc.Extent.height : descriptor.extent.height;
		descriptor.extent.depth = DepthStencil->Desc.Extent.depth < descriptor.extent.depth ? DepthStencil->Desc.Extent.depth : descriptor.extent.depth;
	}
	descriptor.AttachmentCount = NumColorAttachment;

	return descriptor;
}
