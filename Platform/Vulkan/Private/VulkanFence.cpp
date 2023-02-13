#include "pch.h"
#include "Platform/Vulkan/Public/VulkanFence.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

namespace Vulkan
{

	VulkanFence::VulkanFence(VulkanDevice *pInDevice)
		: pDevice(pInDevice)
	{
		VkFenceCreateInfo info = {};
		{
			info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		}
		RE_ASSERT(vkCreateFence(pDevice->GetHandle(), &info, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &Fence) == VK_SUCCESS, "Failed to create fence.");
	}

	VulkanFence::~VulkanFence()
	{
		vkDestroyFence(pDevice->GetHandle(), Fence, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	void VulkanFence::Reset()
	{
		vkResetFences(pDevice->GetHandle(), 1, &Fence);
	}

	int32 VulkanFence::Wait(double Time)
	{
		return vkWaitForFences(pDevice->GetHandle(), 1, &Fence, VK_TRUE, Time) == VK_SUCCESS;
	}

}