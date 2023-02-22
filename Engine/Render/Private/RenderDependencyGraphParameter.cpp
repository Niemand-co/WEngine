#include "pch.h"
#include "Render/Public/RenderDependencyGraphParameter.h"

inline EAccess GetPassAccess(EPassFlag Flag, EAccess& SRVAccess, EAccess& UAVAccess)
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

	for (uint32 Index = Members.Size() - 1; Index >= 0; --Index)
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
	const WRDGRenderTargetBindingSlots& RenderTarget = GetRenderTarget();
	uint32 ColorAttachmentCount = 0;
	for (uint32 ColorAttachmentIndex = 0; ColorAttachmentIndex < MaxSimultaneousRenderTargets; ++ColorAttachmentIndex)
	{
		if (RenderTarget.ColorTextures[ColorAttachmentIndex].Texture != nullptr)
		{
			WRDGTexture *Texture = RenderTarget.ColorTextures[ColorAttachmentIndex].Texture;
			ColorAttachmentCount++;
			descriptor.AttachmentDescriptors[ColorAttachmentCount].attachmentFormat = Texture->Desc.format;
			descriptor.AttachmentDescriptors[ColorAttachmentCount].sampleCount = Texture->Desc.sampleCount;
		}
	}
	descriptor.AttachmentCount = ColorAttachmentCount;
	return descriptor;
}
