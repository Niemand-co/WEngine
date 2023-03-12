#include "pch.h"
#include "Vulkan/Allocator/Public/VulkanMemoryManager.h"
#include "Vulkan/Public/VulkanDevice.h"
#include "Vulkan/Public/VulkanGPU.h"

namespace Vulkan
{

	VulkanAllocation::VulkanAllocation(VulkanDevice* pInDevice, VkDeviceMemory& InMempry)
		: pDevice(pInDevice), Memory(InMempry), FramesAfterLastUsed(0)
	{
	}

	VulkanAllocation::~VulkanAllocation()
	{
		vkFreeMemory(pDevice->GetHandle(), Memory, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	void VulkanAllocation::BindBuffer(VkBuffer& InBuffer)
	{
		vkBindBufferMemory(pDevice->GetHandle(), InBuffer, Memory, 0);
	}

	VulkanMemoryManager::VulkanMemoryManager(VulkanDevice* pInDevice)
		: pDevice(pInDevice)
	{
		vkGetPhysicalDeviceMemoryProperties(*pDevice->GetGPU()->GetHandle(), &MemoryProperties);
	}

	VulkanMemoryManager::~VulkanMemoryManager()
	{
	}

	void VulkanMemoryManager::Init()
	{
	}

	void VulkanMemoryManager::Tick()
	{
		uint32 NumAllocationToFree = 0;
		for (uint32 AllocationIndex = FreeAllocations.Size(); AllocationIndex >= 0; --AllocationIndex)
		{
			FreeAllocations[AllocationIndex]->FramesAfterLastUsed++;
			if (FreeAllocations[AllocationIndex]->FramesAfterLastUsed == 30)
			{
				FreeAllocations.RemoveAndSwap(AllocationIndex);
			}
			FreeAllocations.Resize(FreeAllocations.Size() - 1);
		}
	}

	void VulkanMemoryManager::AllocateBuffer(VkBuffer& InOutBuffer, VulkanAllocation*& InOutAllocation, VkBufferUsageFlags& InUsageFlags, VkMemoryPropertyFlags& MemoryPropertyFlags)
	{
		VkBufferCreateInfo BufferCreateInfo = {};
		{
			BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			BufferCreateInfo.usage = InUsageFlags;
		}

		RE_ASSERT(vkCreateBuffer(pDevice->GetHandle(), &BufferCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &InOutBuffer) == VK_SUCCESS, "Failed to create buffer.");

		VkMemoryRequirements Requirements;
		vkGetBufferMemoryRequirements(pDevice->GetHandle(), InOutBuffer, &Requirements);

		int32 MemoryIndex = -1;
		for (uint32 MemoryTypeIndex = 0; MemoryTypeIndex < MemoryProperties.memoryHeapCount; ++MemoryTypeIndex)
		{
			if ((Requirements.memoryTypeBits & (0x01 << MemoryTypeIndex) != 0) && ((MemoryProperties.memoryTypes[MemoryTypeIndex].propertyFlags & MemoryPropertyFlags) != 0))
			{
				MemoryIndex = MemoryProperties.memoryTypes[MemoryTypeIndex].heapIndex;
				break;
			}
		}
		RE_ASSERT(MemoryIndex >= 0, "No support memory heap.");

		VkMemoryAllocateInfo MemoryAllocateInfo = {};
		{
			MemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			MemoryAllocateInfo.allocationSize = Requirements.size;
			MemoryAllocateInfo.memoryTypeIndex = MemoryIndex;
		}
		VkDeviceMemory DeviceMemory;
		vkAllocateMemory(pDevice->GetHandle(), &MemoryAllocateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &DeviceMemory);
		
		InOutAllocation = Alloc(DeviceMemory);
		InOutAllocation->BindBuffer(InOutBuffer);
	}

	void VulkanMemoryManager::AllocateImage(VkImage& InOutImage, VkImageUsageFlags& InUsageFlags)
	{
	}

	void VulkanMemoryManager::DeallocateBuffer(VkBuffer& InBuffer, VulkanAllocation*& InAllocation)
	{
		FreeAllocations.Push(InAllocation);
		InAllocation = nullptr;
		vkDestroyBuffer(pDevice->GetHandle(), InBuffer, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	VulkanAllocation* VulkanMemoryManager::Alloc(VkDeviceMemory& Memory)
	{
		return new VulkanAllocation(pDevice, Memory);
	}

}