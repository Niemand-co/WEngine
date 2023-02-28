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
		}

	}

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
			descriptor.ColorAttachmentDescriptors[ColorAttachmentIndex].attachmentFormat = Texture->Desc.format;
			descriptor.ColorAttachmentDescriptors[ColorAttachmentIndex].attachmentLoadOP = LoadOp;
			descriptor.ColorAttachmentDescriptors[ColorAttachmentIndex].attachmentStoreOP = StoreOp;
			descriptor.ColorAttachmentDescriptors[ColorAttachmentIndex].sampleCount = Texture->Desc.sampleCount;
		}

		++ColorAttachmentIndex;
	});
	descriptor.ColorAttachmentCount = ColorAttachmentIndex;

	const WRDGDepthStencilBinding& DepthStencil = RenderTargets.DepthStencilTexture;
	if (!DepthStencil.Texture)
	{
		WRDGTexture *Texture = DepthStencil.Texture;
		AttachmentStoreOP DepthStoreOp = Texture->Desc.IsDepthFormat() ? AttachmentStoreOP::DontCare : AttachmentStoreOP::Store;
		AttachmentStoreOP StencilStoreOp = Texture->Desc.IsStencilFormat() ? AttachmentStoreOP::DontCare : AttachmentStoreOP::Store;
		AttachmentLoadOP DepthLoadOp = DepthStencil.DepthLoadOP;
		AttachmentLoadOP StencilLoadOp = DepthStencil.StencilLoadOP;

		descriptor.DepthStencilAttachmentDescriptor.attachmentFormat = Texture->Desc.format;
		descriptor.DepthStencilAttachmentDescriptor.attachmentLoadOP = DepthLoadOp;
		descriptor.DepthStencilAttachmentDescriptor.attachmentStoreOP = DepthStoreOp;
		descriptor.DepthStencilAttachmentDescriptor.stencilLoadOP = StencilLoadOp;
		descriptor.DepthStencilAttachmentDescriptor.stencilStoreOP = StencilStoreOp;
		descriptor.DepthStencilAttachmentDescriptor.sampleCount = Texture->Desc.sampleCount;
	}

	return descriptor;
}
