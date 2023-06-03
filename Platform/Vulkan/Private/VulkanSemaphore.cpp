#include "pch.h"
#include "Platform/Vulkan/Public/VulkanSemaphore.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

namespace Vulkan
{

	VulkanSemaphore::VulkanSemaphore(VulkanDevice *pInDevice)
		: pDevice(pInDevice)
	{
		VkSemaphoreCreateInfo info = {};
		{
			info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		}
		RE_ASSERT(vkCreateSemaphore(pInDevice->GetHandle(), &info, static_cast<VulkanAllocator*>(GetCPUAllocator())->GetCallbacks(), &Semaphore) == VK_SUCCESS, "Failed to create semaphore.");
	}

	VulkanSemaphore::~VulkanSemaphore()
	{
		vkDestroySemaphore(pDevice->GetHandle(), Semaphore, static_cast<VulkanAllocator*>(GetCPUAllocator())->GetCallbacks());
	}

}