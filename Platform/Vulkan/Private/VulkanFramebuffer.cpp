#include "pch.h"
#include "Platform/Vulkan/Public/VulkanFramebuffer.h"

namespace Vulkan
{

	VulkanFramebuffer::VulkanFramebuffer(VkFramebuffer *framebuffer, VkDevice *pDevice)
		: m_pFramebuffer(framebuffer), m_pDevice(pDevice)
	{
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		  vkDestroyFramebuffer(*m_pDevice, *m_pFramebuffer, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks());
	}

	VkFramebuffer* VulkanFramebuffer::GetHandle()
	{
		return m_pFramebuffer;
	}

}