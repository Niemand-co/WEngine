#include "pch.h"
#include "Vulkan/Allocator/Public/VulkanMemoryManager.h"
#include "Vulkan/Public/VulkanDevice.h"
#include "Vulkan/Public/VulkanGPU.h"
#include "Vulkan/Public/VulkanBuffer.h"

namespace Vulkan
{

	VulkanAllocation::VulkanAllocation(VulkanDevice* pInDevice, VkDeviceMemory& InMempry, uint32 InSize)
		: pDevice(pInDevice), Memory(InMempry), FramesAfterLastUsed(0), Size(InSize)
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
		for (int32 AllocationIndex = FreeAllocations.Size(); AllocationIndex >= 0; --AllocationIndex)
		{
			FreeAllocations[AllocationIndex]->FramesAfterLastUsed++;
			if (FreeAllocations[AllocationIndex]->FramesAfterLastUsed >= 30)
			{
				VulkanAllocation *Allocation = FreeAllocations[AllocationIndex];
				FreeAllocations.RemoveAndSwap(AllocationIndex);
				delete Allocation;
			}
			FreeAllocations.Resize(FreeAllocations.Size() - 1);
		}
	}

	void VulkanMemoryManager::AllocateBuffer(VkBuffer& InOutBuffer, VulkanAllocation*& InOutAllocation, uint32 InSize, VkBufferUsageFlags& InUsageFlags, VkMemoryPropertyFlags& MemoryPropertyFlags)
	{
		VkBufferCreateInfo BufferCreateInfo = {};
		{
			BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			BufferCreateInfo.usage = InUsageFlags;
			BufferCreateInfo.size = InSize;
		}

		RE_ASSERT(vkCreateBuffer(pDevice->GetHandle(), &BufferCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &InOutBuffer) == VK_SUCCESS, "Failed to create buffer.");

		VkMemoryRequirements Requirements;
		vkGetBufferMemoryRequirements(pDevice->GetHandle(), InOutBuffer, &Requirements);

		int32 MemoryIndex = -1;
		for (uint32 MemoryTypeIndex = 0; MemoryTypeIndex < MemoryProperties.memoryHeapCount; ++MemoryTypeIndex)
		{
			if ((Requirements.memoryTypeBits & (0x01 << MemoryTypeIndex) != 0) && ((MemoryProperties.memoryTypes[MemoryTypeIndex].propertyFlags & MemoryPropertyFlags) == MemoryPropertyFlags))
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
		RE_ASSERT(vkAllocateMemory(pDevice->GetHandle(), &MemoryAllocateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &DeviceMemory) == VK_SUCCESS, "Failed to allocate memory.");
		
		InOutAllocation = Alloc(DeviceMemory, Requirements.size);
		InOutAllocation->BindBuffer(InOutBuffer);
		//UsedAllocations.Push(InOutAllocation);
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

	VulkanAllocation* VulkanMemoryManager::Alloc(VkDeviceMemory& Memory, uint32 InSize)
	{
		for (uint32 AllocationIndex = 0; AllocationIndex < FreeAllocations.Size(); ++AllocationIndex)
		{
			if (FreeAllocations[AllocationIndex]->Size == InSize)
			{
				VulkanAllocation* Allocation = FreeAllocations[AllocationIndex];
				FreeAllocations.RemoveAndSwap(AllocationIndex);
				return Allocation;
			}
		}

		return new VulkanAllocation(pDevice, Memory, InSize);
	}

	VulkanStagingBufferManager::VulkanStagingBufferManager(VulkanDevice *pInDevice)
		: pDevice(pInDevice)
	{
		vkGetPhysicalDeviceMemoryProperties(*pDevice->GetGPU()->GetHandle(), &MemoryProperties);
	}

	VulkanStagingBufferManager::~VulkanStagingBufferManager()
	{
	}

	void VulkanStagingBufferManager::Tick()
	{
		uint32 NumAllocationToFree = 0;
		for (uint32 AllocationIndex = FreeAllocations.Size(); AllocationIndex >= 0; --AllocationIndex)
		{
			FreeAllocations[AllocationIndex]->FramesAfterLastUsed++;
			if (FreeAllocations[AllocationIndex]->FramesAfterLastUsed >= 30)
			{
				VulkanAllocation* Allocation = FreeAllocations[AllocationIndex];
				FreeAllocations.RemoveAndSwap(AllocationIndex);
				delete Allocation;
			}
			FreeAllocations.Resize(FreeAllocations.Size() - 1);
		}
	}

	VulkanStagingBuffer* VulkanStagingBufferManager::AcquireBuffer(uint32 Size, VkBufferUsageFlags UsageFlags, VkMemoryPropertyFlags MemoryPropertyFlags)
	{
		VkBufferCreateInfo BufferCreateInfo = {};
		{
			BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			BufferCreateInfo.size = Size;
			BufferCreateInfo.usage = UsageFlags;
		}

		VkBuffer Buffer;
		vkCreateBuffer(pDevice->GetHandle(), &BufferCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &Buffer);

		VkMemoryRequirements Requirements;
		vkGetBufferMemoryRequirements(pDevice->GetHandle(), Buffer, &Requirements);

		int32 MemoryIndex = -1;
		for (uint32 MemoryTypeIndex = 0; MemoryTypeIndex < MemoryProperties.memoryTypeCount; ++MemoryTypeIndex)
		{
			if ((Requirements.memoryTypeBits & (0x01 << MemoryTypeIndex) != 0) && ((MemoryProperties.memoryTypes[MemoryTypeIndex].propertyFlags & MemoryPropertyFlags) == MemoryPropertyFlags))
			{
				MemoryIndex = MemoryProperties.memoryTypes[MemoryTypeIndex].heapIndex;
				break;
			}
		}
		RE_ASSERT(MemoryIndex >= 0, "No support memory heap.");

		VkMemoryAllocateInfo MemoryAllocateInfo = {};
		{
			MemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			MemoryAllocateInfo.memoryTypeIndex = MemoryIndex;
			MemoryAllocateInfo.allocationSize = Requirements.size;
		}

		VkDeviceMemory Memory;
		vkAllocateMemory(pDevice->GetHandle(), &MemoryAllocateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &Memory);

		VulkanAllocation *Allocation = Alloc(Memory, Requirements.size);
		Allocation->BindBuffer(Buffer);
		return new VulkanStagingBuffer(pDevice, Buffer, Allocation);
	}

	void VulkanStagingBufferManager::ReleaseBuffer(VulkanStagingBuffer* StagingBuffer)
	{
		FreeAllocations.Push(StagingBuffer->Allocation);
		delete StagingBuffer;
	}

	VulkanAllocation* VulkanStagingBufferManager::Alloc(VkDeviceMemory& Memory, uint32 InSize)
	{
		for (uint32 AllocationIndex = 0; AllocationIndex < FreeAllocations.Size(); ++AllocationIndex)
		{
			if (FreeAllocations[AllocationIndex]->Size == InSize)
			{
				VulkanAllocation* Allocation = FreeAllocations[AllocationIndex];
				FreeAllocations.RemoveAndSwap(AllocationIndex);
				return Allocation;
			}
		}

		return new VulkanAllocation(pDevice, Memory, InSize);
	}

}