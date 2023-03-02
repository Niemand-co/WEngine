#include "pch.h"
#include "Platform/Vulkan/Public/VulkanFramebuffer.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

namespace Vulkan
{

	VulkanFramebuffer::VulkanFramebuffer(VulkanDevice *pInDevice, VkFramebufferCreateInfo *pInfo)
		: pDevice(pInDevice)
	{
		vkCreateFramebuffer(pDevice->GetHandle(), pInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &Framebuffer);
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		vkDestroyFramebuffer(pDevice->GetHandle(), Framebuffer, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

}