#include "pch.h"
#include "Vulkan/Allocator/Public/VulkanMemoryManager.h"
#include "Vulkan/Public/VulkanDevice.h"
#include "Vulkan/Public/VulkanGPU.h"
#include "Vulkan/Public/VulkanBuffer.h"

namespace Vulkan
{

	VulkanAllocation::VulkanAllocation(VulkanDevice* pInDevice, VkDeviceMemory& InMemory, uint32 InSize)
		: pDevice(pInDevice), Memory(InMemory), FramesAfterLastUsed(0), Size(InSize)
	{
	}

	VulkanAllocation::~VulkanAllocation()
	{
		vkFreeMemory(pDevice->GetHandle(), Memory, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	void VulkanAllocation::Init(VulkanDevice* pInDevice, VkDeviceMemory& InMemory, uint32 InSize)
	{
		pDevice = pInDevice;
		Memory = InMemory;
		Size = InSize;
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

	void VulkanMemoryManager::ProcessPendingUBFrees()
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

		int32 MemoryTypeIndex = 0;
		for (; MemoryTypeIndex < MemoryProperties.memoryHeapCount; ++MemoryTypeIndex)
		{
			if ((Requirements.memoryTypeBits & (0x01 << MemoryTypeIndex) != 0) && ((MemoryProperties.memoryTypes[MemoryTypeIndex].propertyFlags & MemoryPropertyFlags) == MemoryPropertyFlags))
			{
				break;
			}
		}
		RE_ASSERT(MemoryTypeIndex < MemoryProperties.memoryTypeCount, "No support memory heap.");

		VkMemoryAllocateInfo MemoryAllocateInfo = {};
		{
			MemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			MemoryAllocateInfo.allocationSize = Requirements.size;
			MemoryAllocateInfo.memoryTypeIndex = MemoryTypeIndex;
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

	void VulkanMemoryManager::AllocateUniformBuffer(VulkanAllocation& OutAllocation, uint32 Size, const void* Contents)
	{
		if(!AllocateBufferPooled(OutAllocation, Size, 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, ));
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

	bool VulkanMemoryManager::AllocateBufferPooled(VulkanAllocation& OutAllocation, uint32 Size, uint32 MinAlignment, VkBufferUsageFlags UsageFlags, VkMemoryPropertyFlags MemortPropertyFlags)
	{
		uint32 Alignment = WEngine::Max(MinAlignment, 0u);

		int32 PoolSize = (int32)GetPoolSize(Size, Alignment);
		if (PoolSize != (int32)EPoolSizes::SizeCount)
		{
			Size = PoolSizes[PoolSize];
		}

		WEngine::WScopeLock ScopeLock(&UsedBufferSection);

		for (int32 Index = 0; Index < UsedBufferAllocations[PoolSize].Size(); ++Index)
		{
			VulkanSubresourceAllocation *SubresourceAllocation = UsedBufferAllocations[PoolSize][Index];
			if ((SubresourceAllocation->BufferUsageFlags & UsageFlags) == UsageFlags &&
				(SubresourceAllocation->MemoryPropertyFlags & MemortPropertyFlags) == MemortPropertyFlags)
			{

			}
		}

		return false;
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

		int32 MemoryTypeIndex = 0;
		for (; MemoryTypeIndex < MemoryProperties.memoryTypeCount; ++MemoryTypeIndex)
		{
			if ((Requirements.memoryTypeBits & (0x01 << MemoryTypeIndex) != 0) && ((MemoryProperties.memoryTypes[MemoryTypeIndex].propertyFlags & MemoryPropertyFlags) == MemoryPropertyFlags))
			{
				break;
			}
		}
		RE_ASSERT(MemoryTypeIndex < MemoryProperties.memoryTypeCount, "No support memory heap.");

		VkMemoryAllocateInfo MemoryAllocateInfo = {};
		{
			MemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			MemoryAllocateInfo.memoryTypeIndex = MemoryTypeIndex;
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

	VulkanSubresourceAllocation::VulkanSubresourceAllocation(EVulkanAllocationType InType, class VulkanMemoryManager* InOwner, uint32 InUsedSize, uint32 InBufferId, VkBufferUsageFlags InUsageFlags, VkMemoryPropertyFlags InMemoryPropertyFlags)
		: Type(InType),
		  Owner(InOwner),
		  UsedSize(InUsedSize),
		  BufferId(InBufferId),
		  BufferUsageFlags(InUsageFlags),
		  MemoryPropertyFlags(InMemoryPropertyFlags)
	{
	}

	VulkanSubresourceAllocation::~VulkanSubresourceAllocation()
	{
	}

}