#pragma once
#include "Utils/Allocator/Public/Allocator.h"

namespace Vulkan
{

	class VulkanAllocator : public NormalAllocator
	{
	public:

		VulkanAllocator();

		virtual ~VulkanAllocator();

		VkAllocationCallbacks* GetCallbacks(void *pUserData = nullptr);

	private:

		VkAllocationCallbacks *m_pAllocationCallbacks;

	};

}