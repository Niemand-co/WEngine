#include "pch.h"
#include "Platform/Vulkan/Public/VulkanRenderPass.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

namespace Vulkan
{

	VulkanRenderPass::VulkanRenderPass(class VulkanDevice* pInDevice, VkRenderPassCreateInfo* pInfo)
		: pDevice(pInDevice)
	{
		vkCreateRenderPass(pDevice->GetHandle(), pInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &RenderPass);
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
		vkDestroyRenderPass(pDevice->GetHandle(), RenderPass, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	void VulkanRenderPass::Begin(VulkanCommandBuffer* CmdBuffer)
	{
	}

	void VulkanRenderPass::End(VulkanCommandBuffer* CmdBuffer)
	{
	}

}