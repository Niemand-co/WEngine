#include "pch.h"
#include "Platform/Vulkan/Public/VulkanRenderPass.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

namespace Vulkan
{

	VulkanRenderPass::VulkanRenderPass(class VulkanDevice* pInDevice, VkRenderPassCreateInfo* pInfo)
		: pDevice(pInDevice)
	{
		vkCreateRenderPass(pDevice->GetHandle(), pInfo, ResourceCast(NormalAllocator::Get())->GetCallbacks(), &RenderPass);
	}

	VulkanRenderPass::VulkanRenderPass(VulkanDevice* pInDevice, const VulkanRenderTargetLayout& InRTLayout)
		: pDevice(pInDevice)
	{
		VkRenderPassCreateInfo RenderPassCreateInfo;
		ZeroVulkanStruct(RenderPassCreateInfo, VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO);
		RenderPassCreateInfo.attachmentCount = InRTLayout.NumAttachments;
		RenderPassCreateInfo.pAttachments = InRTLayout.Attachments;
		
		uint32 NumSubpass = 0;
		uint32 NumDependencies = 0;
		VkSubpassDescription Subpasses[8];
		VkSubpassDependency Dependencies[8];
		RenderPassCreateInfo.pSubpasses = Subpasses;
		RenderPassCreateInfo.pDependencies = Dependencies;

		{
			VkSubpassDescription& Subpass = Subpasses[NumSubpass++];
			Subpass.colorAttachmentCount = InRTLayout.NumColorAttachments;
			Subpass.pColorAttachments = InRTLayout.ColorReferences;
			Subpass.pDepthStencilAttachment = InRTLayout.bHasDepthStencilAttachment ? &InRTLayout.DepthStencilReference : nullptr;
			Subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			Subpass.pResolveAttachments = InRTLayout.bHasResolveAttachments ? InRTLayout.ResolveReferences : nullptr;
		}

		RenderPassCreateInfo.subpassCount = NumSubpass;
		RenderPassCreateInfo.dependencyCount = NumDependencies;

		vkCreateRenderPass(pInDevice->GetHandle(), &RenderPassCreateInfo, ResourceCast(NormalAllocator::Get())->GetCallbacks(), &RenderPass);
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
		vkDestroyRenderPass(pDevice->GetHandle(), RenderPass, ResourceCast(NormalAllocator::Get())->GetCallbacks());
	}

	void VulkanRenderPass::Begin(VulkanCommandBuffer* CmdBuffer)
	{
	}

	void VulkanRenderPass::End(VulkanCommandBuffer* CmdBuffer)
	{
	}

}