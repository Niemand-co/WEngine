#include "pch.h"
#include "Platform/Vulkan/Public/VulkanRHIPrivate.h"

namespace Vulkan
{

	struct HashStruct
	{
		uint8 NumAttachments;
		uint8 NumSamples;
		VkFormat Formats[MaxSimultaneousRenderTargets + 1];
		VkAttachmentLoadOp  LoadOps[MaxSimultaneousRenderTargets + 1];
		VkAttachmentStoreOp StoreOps[MaxSimultaneousRenderTargets + 1];
		VkImageLayout  Layouts[MaxSimultaneousRenderTargets];
	};

	VulkanRenderTargetLayout::VulkanRenderTargetLayout(const RHIGraphicsPipelineStateInitializer& Initializer)
		: NumColorAttachments(0),
		  NumInputAttachments(0),
		  bHasResolveAttachments(false),
		  bHasDepthStencilAttachment(false)
	{
		WEngine::Memzero(Attachments, (2 * MaxSimultaneousRenderTargets + 2) * sizeof(VkAttachmentReference));
		WEngine::Memzero(ColorReferences, MaxSimultaneousRenderTargets * sizeof(VkAttachmentReference));
		WEngine::Memzero(InputReferences, (MaxSimultaneousRenderTargets + 1) * sizeof(VkAttachmentReference));
		WEngine::Memzero(ResolveReferences, MaxSimultaneousRenderTargets * sizeof(VkAttachmentReference));
		WEngine::Memzero(&DepthStencilReference, sizeof(VkAttachmentReference));

		HashStruct hashStruct;
		bool bHasClearOp = false;

		for (uint32 Index = 0; Index < Initializer.RenderTargetEnabled; ++Index)
		{
			EFormat Format = (EFormat)Initializer.RenderTargetFormats[Index];
			if (Format != EFormat::Unknown)
			{
				VkAttachmentDescription& Attachment = Attachments[Index];
				Attachment.format = WEngine::ToVulkan(Format);
				Attachment.samples = (VkSampleCountFlagBits)Initializer.NumSamples;
				Attachment.loadOp = WEngine::ToVulkan((EAttachmentLoadOP)Initializer.RenderTargetLoadOps[Index]);
				Attachment.storeOp = WEngine::ToVulkan((EAttachmentStoreOP)Initializer.RenderTargetStoreOps[Index]);
				Attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				Attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

				Attachment.initialLayout = WEngine::ToVulkan((EAttachmentLayout)Initializer.RenderTargetInitialLayouts[Index]);
				Attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				ColorReferences[NumColorAttachments].attachment = NumAttachments;
				ColorReferences[NumColorAttachments].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				if (Attachment.initialLayout == VK_ATTACHMENT_LOAD_OP_CLEAR)
				{
					bHasClearOp = true;
				}

				hashStruct.Formats[Index] = Attachment.format;
				hashStruct.LoadOps[Index] = Attachment.loadOp;
				hashStruct.StoreOps[Index] = Attachment.storeOp;
				hashStruct.Layouts[Index] = Attachment.initialLayout;

				++NumAttachments;
				++NumColorAttachments;
			}
		}

		if (Initializer.DepthStencilFormat != EFormat::Unknown)
		{
			VkAttachmentDescription& Attachment = Attachments[NumAttachments];
			Attachment.format = WEngine::ToVulkan(Initializer.DepthStencilFormat);
			Attachment.loadOp = WEngine::ToVulkan(Initializer.DepthTargetLoadAction);
			Attachment.storeOp = WEngine::ToVulkan(Initializer.DepthTargetStoreAction);
			Attachment.stencilLoadOp = WEngine::ToVulkan(Initializer.StencilTargetLoadAction);
			Attachment.stencilStoreOp = WEngine::ToVulkan(Initializer.StencilTargetStoreAction);

			Attachment.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			Attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			DepthStencilReference.attachment = NumAttachments;
			DepthStencilReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			if (Attachment.loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR || Attachment.stencilLoadOp == VK_ATTACHMENT_LOAD_OP_CLEAR)
			{
				bHasClearOp = true;
			}

			hashStruct.Formats[NumAttachments] = Attachment.format;
			hashStruct.LoadOps[NumAttachments] = Attachment.loadOp;
			hashStruct.StoreOps[NumAttachments] = Attachment.storeOp;

			++NumAttachments;
			bHasDepthStencilAttachment = true;
		}

		hashStruct.NumAttachments = NumAttachments;
		hashStruct.NumSamples = Initializer.NumSamples;

		Hash = WEngine::MemCrc32(&hashStruct, sizeof(HashStruct));
		NumUsedClearValues = bHasClearOp ? NumAttachments : 0;
	}

}