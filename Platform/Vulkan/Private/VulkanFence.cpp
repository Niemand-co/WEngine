#include "pch.h"
#include "Platform/Vulkan/Public/VulkanFence.h"

VulkanFence::VulkanFence(VkFence* fence)
	: m_fence(fence)
{
}

VulkanFence::~VulkanFence()
{
}
