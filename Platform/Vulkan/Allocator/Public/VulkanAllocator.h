#pragma once
#include "Utils/Allocator/Public/Allocator.h"

namespace Vulkan
{

	class VulkanAllocator : public WEngine::Allocator
	{
	public:

		VulkanAllocator();

		virtual ~VulkanAllocator();

		VkAllocationCallbacks* GetCallbacks(void *pUserData = nullptr);

	private:

		VkAllocationCallbacks *m_pAllocationCallbacks;

	};

	struct VulkanAllocatorData
	{
		WEngine::Allocator *allocator;
	};

}