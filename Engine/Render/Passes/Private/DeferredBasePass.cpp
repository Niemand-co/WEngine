#include "pch.h"
#include "Render/Passes/Public/DeferredBasePass.h"
#include "Render/Descriptor/Public/RHIRenderPassDescriptor.h"

DeferredBasePass::DeferredBasePass()
{
}

DeferredBasePass::~DeferredBasePass()
{
}

void DeferredBasePass::InitRHIResource()
{
	RHIRenderPassDescriptor descriptor = {};
	{
		descriptor.AttachmentCount = 5;
		descriptor.SubpassCount = 1;
		descriptor.DependencyCount = 1;
	}
	{
		descriptor.AttachmentDescriptors[0] = { Format::A16R16G16B16_SFloat, 1, AttachmentLoadOP::DontCare, AttachmentStoreOP::Store, AttachmentLoadOP::Load, AttachmentStoreOP::Store, AttachmentLayout::Undefined };
		descriptor.AttachmentDescriptors[1] = { Format::A16R16G16B16_SFloat, 1, AttachmentLoadOP::DontCare, AttachmentStoreOP::Store, AttachmentLoadOP::Load, AttachmentStoreOP::Store, AttachmentLayout::Undefined };
		descriptor.AttachmentDescriptors[2] = { Format::A16R16G16B16_SFloat, 1, AttachmentLoadOP::DontCare, AttachmentStoreOP::Store, AttachmentLoadOP::Load, AttachmentStoreOP::Store, AttachmentLayout::Undefined };
		descriptor.AttachmentDescriptors[3] = { Format::A16R16G16B16_SFloat, 1, AttachmentLoadOP::DontCare, AttachmentStoreOP::Store, AttachmentLoadOP::Load, AttachmentStoreOP::Store, AttachmentLayout::Undefined };
		descriptor.AttachmentDescriptors[4] = { Format::D16_Unorm, 1, AttachmentLoadOP::DontCare, AttachmentStoreOP::Store, AttachmentLoadOP::Load, AttachmentStoreOP::Store, AttachmentLayout::DepthBuffer };
	};
	{
		descriptor.DependencyDescriptors[0] = { -1, PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT, ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE, 0, PIPELINE_STAGE_EARLY_FRAGMENT_TESTS, ACCESS_DEPTH_STENCIL_ATTACHMENT_READ };
		descriptor.DependencyDescriptors[0] = { 0, PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT, ACCESS_COLOR_ATTACHMENT_WRITE, -1, PIPELINE_STAGE_FRAGMENT_SHADER, ACCESS_INPUT_ATTACHMENT_READ };
	}
	{
		descriptor.SubPassDescriptors[0].ColorAttachmentCount = 4;
		descriptor.SubPassDescriptors[0].ColorAttachments[0] = { 0, AttachmentLayout::ColorBuffer };
		descriptor.SubPassDescriptors[0].ColorAttachments[1] = { 1, AttachmentLayout::ColorBuffer };
		descriptor.SubPassDescriptors[0].ColorAttachments[2] = { 2, AttachmentLayout::ColorBuffer };
		descriptor.SubPassDescriptors[0].ColorAttachments[3] = { 3, AttachmentLayout::ColorBuffer };
		descriptor.SubPassDescriptors[0].DepthStencilAttachment = { 4, AttachmentLayout::DepthBuffer };
	}
	RenderPass = GetRenderCommandList()->CreateRenderPass(&descriptor);
}

void DeferredBasePass::ReleaseRHIResource()
{
}

void DeferredBasePass::UpdateRHIResource()
{
}
