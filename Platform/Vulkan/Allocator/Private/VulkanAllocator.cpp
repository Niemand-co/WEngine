#include "pch.h"
#include "Platform/Vulkan/Allocator/Public/VulkanAllocator.h"

namespace Vulkan
{

	void* __stdcall AllocationCallbacks(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
	{
		void *result = GetCPUAllocator()->Allocate(size);
		return result;
	}

	void __stdcall FreeCallbacks(void* pUserData, void* pMemory)
	{
		GetCPUAllocator()->Deallocate(pMemory);
	}

	void* __stdcall ReallocationCallbacks(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
	{
		void* result = GetCPUAllocator()->Reallocate(pOriginal, size);
		return result;
	}

	VulkanAllocator::VulkanAllocator()
	{
		m_pAllocationCallbacks = new VkAllocationCallbacks();
		m_pAllocationCallbacks->pfnAllocation = AllocationCallbacks;
		m_pAllocationCallbacks->pfnFree = FreeCallbacks;
		m_pAllocationCallbacks->pfnReallocation = ReallocationCallbacks;
	}

	VulkanAllocator::~VulkanAllocator()
	{
	}

	VkAllocationCallbacks* VulkanAllocator::GetCallbacks(void* pUserData)
	{
		if(pUserData != nullptr)
			m_pAllocationCallbacks->pUserData = pUserData;
		
		return m_pAllocationCallbacks;
	}

}