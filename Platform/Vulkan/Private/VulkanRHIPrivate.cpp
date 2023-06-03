#include "pch.h"
#include "Platform/Vulkan/Public/VulkanRHIPrivate.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanRenderPass.h"
#include "Platform/Vulkan/Public/VulkanFramebuffer.h"
#include "Render/Descriptor/Public/RHIRenderPassDescriptor.h"
#include "Render/Descriptor/Public/RHIFramebufferDescriptor.h"

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
		: NumAttachments(0),
		  NumColorAttachments(0),
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
		WEngine::Memzero(hashStruct);
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

				Attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
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

	VulkanRenderTargetLayout::VulkanRenderTargetLayout(const RHIRenderPassDescriptor* Descriptor)
		: NumAttachments(0),
		  NumColorAttachments(0),
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
		WEngine::Memzero(hashStruct);
		bool bHasClearOp = false;
		uint32 NumSamples = 0;

		for (uint32 Index = 0; Index < Descriptor->ColorAttachmentCount; ++Index)
		{
			EFormat Format = (EFormat)Descriptor->ColorAttachmentDescriptors[Index].attachmentFormat;
			if (Format != EFormat::Unknown)
			{
				NumSamples = Descriptor->ColorAttachmentDescriptors[Index].sampleCount;
				VkAttachmentDescription& Attachment = Attachments[Index];
				Attachment.format = WEngine::ToVulkan(Format);
				Attachment.samples = WEngine::ToVulkan(NumSamples);
				Attachment.loadOp = WEngine::ToVulkan((EAttachmentLoadOP)Descriptor->ColorAttachmentDescriptors[Index].attachmentLoadOP);
				Attachment.storeOp = WEngine::ToVulkan((EAttachmentStoreOP)Descriptor->ColorAttachmentDescriptors[Index].attachmentStoreOP);
				Attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				Attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

				Attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
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

		if (Descriptor->bHasDepthStencilAttachment)
		{
			NumSamples = Descriptor->DepthStencilAttachmentDescriptor.sampleCount;
			VkAttachmentDescription& Attachment = Attachments[NumAttachments];
			Attachment.format = WEngine::ToVulkan(Descriptor->DepthStencilAttachmentDescriptor.attachmentFormat);
			Attachment.samples = WEngine::ToVulkan(NumSamples);
			Attachment.loadOp = WEngine::ToVulkan(Descriptor->DepthStencilAttachmentDescriptor.attachmentLoadOP);
			Attachment.storeOp = WEngine::ToVulkan(Descriptor->DepthStencilAttachmentDescriptor.attachmentStoreOP);
			Attachment.stencilLoadOp = WEngine::ToVulkan(Descriptor->DepthStencilAttachmentDescriptor.stencilLoadOP);
			Attachment.stencilStoreOp = WEngine::ToVulkan(Descriptor->DepthStencilAttachmentDescriptor.stencilStoreOP);

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
		hashStruct.NumSamples = NumSamples;

		Hash = WEngine::MemCrc32(&hashStruct, sizeof(HashStruct));
		NumUsedClearValues = bHasClearOp ? NumAttachments : 0;
	}

	VulkanRenderPass* VulkanLayoutManager::GetOrCreateRenderPass(const VulkanRenderTargetLayout& RTLayout)
	{
		uint32 RenderPassHash = RTLayout.Hash;
		VulkanRenderPass *RenderPass = nullptr;
		{
			WEngine::WScopeLock Lock(&RenderPassLock);
			if (RenderPasses.Find(RenderPassHash))
			{
				RenderPass = RenderPasses[RenderPassHash];
				return RenderPass;
			}
		}
		
		RenderPass = new VulkanRenderPass(pDevice, RTLayout);
		{
			WEngine::WScopeLock Loc(&RenderPassLock);
			if (RenderPasses.Find(RenderPassHash))
			{
				delete RenderPass;
				return RenderPasses[RenderPassHash];
			}
			RenderPasses.Insert(RenderPassHash, RenderPass);
		}
		return RenderPass;
	}

	VulkanFramebuffer* VulkanLayoutManager::GetOrCreateFramebuffer(const RHIFramebufferDescriptor* RTInfo, const VulkanRenderTargetLayout& RTLayout, VulkanRenderPass *RenderPass)
	{
		uint32 FramebufferHash = RTLayout.Hash;
		FramebufferList *NewFramebuffer = nullptr;
		{
			WEngine::WScopeLock Lock(&FramebufferLock);
			if (Framebuffers.Find(FramebufferHash))
			{
				NewFramebuffer = Framebuffers[FramebufferHash];
				for (VulkanFramebuffer* Framebuffer : NewFramebuffer->Framebuffer)
				{
					VkRect2D RenderArea = Framebuffer->GetRenderArea();

					if (Framebuffer->MatchInfo(RTInfo) &&
					RenderArea.extent.width == RTInfo->Extent.width && RenderArea.extent.height == RTInfo->Extent.height &&
					RenderArea.offset.x == RTInfo->Offset.x && RenderArea.offset.y == RTInfo->Offset.y)
					{
						return Framebuffer;
					}
				}
			}
			else
			{
				NewFramebuffer = new FramebufferList;
				Framebuffers.Insert(FramebufferHash, NewFramebuffer);
			}
		}


		VulkanFramebuffer *Framebuffer = new VulkanFramebuffer(pDevice, RTInfo, RTLayout, RenderPass);
		NewFramebuffer->Framebuffer.Push(Framebuffer);
		return Framebuffer;
	}

}