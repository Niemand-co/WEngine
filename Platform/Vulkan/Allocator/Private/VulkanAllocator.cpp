#include "pch.h"
#include "Platform/Vulkan/Allocator/Public/VulkanAllocator.h"

namespace Vulkan
{

	void* __stdcall AllocationCallbacks(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
	{
		void *result = ((VulkanAllocatorData*)pUserData)->allocator->Allocate(size);
		return result;
	}

	void __stdcall FreeCallbacks(void* pUserData, void* pMemory)
	{
		((VulkanAllocatorData*)pUserData)->allocator->Deallocate(pMemory, ((VulkanAllocatorData*)pUserData)->size);
	}

	void* __stdcall ReallocationCallbacks(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
	{
		void* result = ((VulkanAllocatorData*)pUserData)->allocator->Reallocate(pOriginal, 0, size);
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
		m_pAllocationCallbacks->pUserData = pUserData;
		
		return m_pAllocationCallbacks;
	}

}