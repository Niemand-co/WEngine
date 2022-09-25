#include "pch.h"
#include "Platform/Vulkan/Public/VulkanRenderTarget.h"

VulkanRenderTarget::VulkanRenderTarget(VkFramebuffer* framebuffer, unsigned int width, unsigned int height)
	: m_framebuffer(framebuffer)
{
	m_width = width;
	m_height = height;
}

VulkanRenderTarget::~VulkanRenderTarget()
{
}

VkFramebuffer* VulkanRenderTarget::GetHandle()
{
	return m_framebuffer;
}
