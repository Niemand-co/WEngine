#include "pch.h"
#include "Platform/Vulkan/Public/VulkanRenderTarget.h"

namespace Vulkan
{

	VulkanRenderTarget::VulkanRenderTarget(VkFramebuffer* framebuffer, unsigned int width, unsigned int height, VkDevice *device)
		: m_framebuffer(framebuffer), m_pDevice(device)
	{
		m_width = width;
		m_height = height;
	}

	VulkanRenderTarget::~VulkanRenderTarget()
	{
		vkDestroyFramebuffer(*m_pDevice, *m_framebuffer, static_cast<VulkanAllocator*>(GetCPUAllocator())->GetCallbacks());
	}

	VkFramebuffer* VulkanRenderTarget::GetHandle()
	{
		return m_framebuffer;
	}

}