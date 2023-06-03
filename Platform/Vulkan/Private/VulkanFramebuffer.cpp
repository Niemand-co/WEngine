#include "pch.h"
#include "Platform/Vulkan/Public/VulkanFramebuffer.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanRHIPrivate.h"
#include "Platform/Vulkan/Public/VulkanRenderPass.h"
#include "Platform/Vulkan/Public/VulkanTexture.h"
#include "Render/Descriptor/Public/RHIFramebufferDescriptor.h"

namespace Vulkan
{

	VulkanFramebuffer::VulkanFramebuffer(VulkanDevice *pInDevice, const RHIFramebufferDescriptor* RTInfo, const VulkanRenderTargetLayout& RTLayout, VulkanRenderPass *RenderPass)
		: pDevice(pInDevice),
		  NumAttachments(0),
		  NumColorAttachments(0),
		  bHasDepthStencilAttachment(false)
	{
		VkFramebufferCreateInfo FramebufferCreateInfo;
		ZeroVulkanStruct(FramebufferCreateInfo, VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO);
		FramebufferCreateInfo.renderPass = RenderPass->GetHandle();
		
		for (int32 Index = 0; Index < RTLayout.NumColorAttachments; ++Index)
		{
			Attachments[Index] = ResourceCast(RTInfo->Attachments[NumAttachments])->GetViewHandle();
			ColorAttachments[Index] = ResourceCast(RTInfo->Attachments[NumAttachments])->GetHandle();
			NumAttachments++;
			NumColorAttachments++;
		}

		if (RTLayout.bHasDepthStencilAttachment)
		{
			Attachments[NumAttachments] = ResourceCast(RTInfo->Attachments[NumAttachments])->GetViewHandle();
			DepthStencilAttachment = ResourceCast(RTInfo->Attachments[NumAttachments])->GetHandle();
			NumAttachments++;
			bHasDepthStencilAttachment = true;
		}

		RenderArea.extent.width = RTInfo->Extent.width;
		RenderArea.extent.height = RTInfo->Extent.height;
		RenderArea.offset.x = RTInfo->Offset.x;
		RenderArea.offset.y = RTInfo->Offset.y;

		FramebufferCreateInfo.width = RTInfo->Extent.width;
		FramebufferCreateInfo.height = RTInfo->Extent.height;
		FramebufferCreateInfo.layers = RTInfo->Extent.depth;
		FramebufferCreateInfo.attachmentCount = NumAttachments;
		FramebufferCreateInfo.pAttachments = Attachments;

		vkCreateFramebuffer(pDevice->GetHandle(), &FramebufferCreateInfo, static_cast<VulkanAllocator*>(GetCPUAllocator())->GetCallbacks(), &Framebuffer);
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		vkDestroyFramebuffer(pDevice->GetHandle(), Framebuffer, static_cast<VulkanAllocator*>(GetCPUAllocator())->GetCallbacks());
	}

	bool VulkanFramebuffer::MatchInfo(const RHIFramebufferDescriptor* RTInfo)
	{
		if (NumAttachments != RTInfo->AttachmentCount)
		{
			return false;
		}

		uint32 MatchingNum = 0;
		for (uint32 Index = 0; Index < NumColorAttachments; ++Index)
		{
			VkImage ImageA = ResourceCast(RTInfo->Attachments[MatchingNum])->GetHandle();
			VkImage ImageB = ColorAttachments[Index];
			if (ImageA != ImageB)
			{
				return false;
			}
			++MatchingNum;
		}

		if (bHasDepthStencilAttachment)
		{
			VkImage ImageA = ResourceCast(RTInfo->Attachments[MatchingNum])->GetHandle();
			VkImage ImageB = DepthStencilAttachment;
			if (ImageA != ImageB)
			{
				return false;
			}
		}

		return true;
	}

}