#include "pch.h"
#include "Platform/Vulkan/Public/VulkanFramebuffer.h"

VulkanFramebuffer::VulkanFramebuffer(VkFramebuffer *framebuffer)
	: m_framebuffer(framebuffer)
{
}

VulkanFramebuffer::~VulkanFramebuffer()
{
}

VkFramebuffer* VulkanFramebuffer::GetHandle()
{
	return m_framebuffer;
}
