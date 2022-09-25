#include "pch.h"
#include "Platform/Vulkan/Public/VulkanSemaphore.h"

VulkanSemaphore::VulkanSemaphore(VkSemaphore *semaphore)
	: m_semaphore(semaphore)
{
}

VulkanSemaphore::~VulkanSemaphore()
{
}

VkSemaphore* VulkanSemaphore::GetHandle()
{
	return m_semaphore;
}
