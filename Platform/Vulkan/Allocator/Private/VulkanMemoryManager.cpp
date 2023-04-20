#include "pch.h"
#include "Vulkan/Allocator/Public/VulkanMemoryManager.h"
#include "Vulkan/Public/VulkanDevice.h"
#include "Vulkan/Public/VulkanGPU.h"
#include "Vulkan/Public/VulkanBuffer.h"

namespace Vulkan
{

	void VulkanAllocation::Init(EVulkanAllocationType InType, EVulkanAllocationMetaType InMetaType, uint32 InSize, uint32 InOffset, uint32 InAllocatorIndex, uint32 InAllocationIndex)
	{
		Type = InType;
		MetaType = InMetaType;
		Size = InSize;
		Offset = InOffset;
		AllocatorIndex = InAllocatorIndex;
		AllocationIndex = InAllocationIndex;
	}

	void* VulkanAllocation::GetMappedPointer(VulkanDevice* pDevice)
	{
		VulkanSubresourceAllocator* Allocator = pDevice->GetMemoryManager()->GetSubresourceAllocator(AllocatorIndex);
		uint8* Pointer = (uint8*)Allocator->GetMappedPointer();
		return Offset + Pointer;
	}

	void VulkanAllocation::FlushMappedMemory(VulkanDevice* pDevice)
	{
		VulkanSubresourceAllocator *Allocator = pDevice->GetMemoryManager()->GetSubresourceAllocator(AllocatorIndex);
		Allocator->FlushMappedMemory(Offset, Size);
	}

	void VulkanAllocation::InvalidateMappedMemory(VulkanDevice* pDevice)
	{
		VulkanSubresourceAllocator* Allocator = pDevice->GetMemoryManager()->GetSubresourceAllocator(AllocatorIndex);
		Allocator->InvalidateMappedMemory(Offset, Size);
	}

	void* VulkanDeviceMemoryAllocation::Map(uint32 MappingSize, uint32 MappingOffset)
	{
		if (!MappedPointer)
		{
			vkMapMemory(Device, Memory, MappingOffset, MappingSize, 0, &MappedPointer);
		}
		return MappedPointer;
	}

	void VulkanDeviceMemoryAllocation::Unmap()
	{
		vkUnmapMemory(Device, Memory);
		MappedPointer = nullptr;
	}

	void VulkanDeviceMemoryAllocation::FlushMappedMemory(VkDeviceSize FlushSize, VkDeviceSize FlushOffset)
	{
		if (!bIsCoherent)
		{
			VkMappedMemoryRange Range = {};
			{
				Range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
				Range.size = FlushSize;
				Range.offset = FlushOffset;
				Range.memory = Memory;
			}
			RE_ASSERT(vkFlushMappedMemoryRanges(Device, 1, &Range) == VK_SUCCESS, "Failed to flush mapped memory.");
		}
	}

	void VulkanDeviceMemoryAllocation::InvalidateMappedMemory(VkDeviceSize InvalidateSize, VkDeviceSize InvalidateOffset)
	{
		if (!bIsCoherent)
		{
			VkMappedMemoryRange Range = {};
			{
				Range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
				Range.size = InvalidateSize;
				Range.offset = InvalidateOffset;
				Range.memory = Memory;
			}
			RE_ASSERT(vkInvalidateMappedMemoryRanges(Device, 1, &Range) == VK_SUCCESS, "Failed to invalidate mapped memory.");
		}
	}

	VulkanSubresourceAllocator::VulkanSubresourceAllocator(EVulkanAllocationType InType, class VulkanMemoryManager* InOwner, VulkanDeviceMemoryAllocation *InDeviceMemoryAllocation, uint32 InBufferSize, uint32 InAlignment, VkBufferUsageFlags InUsageFlags, VkMemoryPropertyFlags InMemoryPropertyFlags, uint32 InPoolSizeIndex)
		: Type(InType),
		  Owner(InOwner),
		  DeviceMemoryAllocation(InDeviceMemoryAllocation),
		  BufferSize(InBufferSize),
		  Alignment(InAlignment),
		  UsedSize(0),
		  Frame(0),
		  BufferUsageFlags(InUsageFlags),
		  MemoryPropertyFlags(InMemoryPropertyFlags),
		  PoolSizeIndex(InPoolSizeIndex)
	{
		WRange FullRange(DeviceMemoryAllocation->GetSize(), 0);
		FreeList.Push(FullRange);
	}

	VulkanSubresourceAllocator::~VulkanSubresourceAllocator()
	{
	}

	bool VulkanSubresourceAllocator::TryAllocate(VulkanAllocation& OutAllocation, uint32 InSize, uint32 InAlignment, EVulkanAllocationMetaType InMetaType)
	{
		WEngine::WScopeLock Lock(&Section);
		
		InAlignment = WEngine::Max(Alignment, InAlignment);
		for (uint32 Index = 0; Index < FreeList.Size(); ++Index)
		{
			WRange &Entry = FreeList[Index];
			uint32 AllocatedOffset = Entry.Offset;
			uint32 AlignedOffset = WEngine::Align(AllocatedOffset, InAlignment);
			uint32 AlignmentAdjustment = AlignedOffset - AllocatedOffset;
			uint32 AllocateSize = AlignmentAdjustment + InSize;
			if (AllocateSize <= Entry.Size)
			{
				if (AllocateSize < Entry.Size)
				{
					Entry.Size -= AllocateSize;
					Entry.Offset += AllocateSize;
				}
				else
				{
					FreeList.RemoveAndSwap(Index);
				}

				OutAllocation.Init(Type, InMetaType, InSize, AlignedOffset, AllocatorIndex, NumAllocations);
				UsedSize += AllocateSize;
				NumAllocations++;
			}
			return true;
		}

		return false;
	}

	void VulkanSubresourceAllocator::Free(VulkanAllocation& Allocation)
	{
		
	}

	bool VulkanSubresourceAllocator::JoinFreeBlocks()
	{
		if (FreeList.Size() == 1 && NumAllocations == 0)
		{
			RE_ASSERT(UsedSize == 0, "Size Error.");
			RE_ASSERT(FreeList[0].Size == BufferSize && FreeList[0].Offset == 0, "");
			return true;
		}
		return false;
	}

	VulkanResourceHeap::VulkanResourceHeap(VulkanMemoryManager* InOwner, uint32 InMemoryTypeIndex)
		: Owner(InOwner),
		  MemoryTypeIndex(InMemoryTypeIndex)
	{

	}

	VulkanResourceHeap::~VulkanResourceHeap()
	{
	}

	void VulkanResourceHeap::TryAllocate()
	{
		WEngine::WScopeLock Lock(&PageCS);


	}

	void VulkanResourceHeap::FreePage()
	{
	}

	void VulkanResourceHeap::ReleasePage()
	{
	}

	uint32 VulkanResourceHeap::GetPageSizeBucket(VulkanPageSizeBucket& OutBucket, uint8 Type, uint32 AllocationSize)
	{
		return uint32();
	}

	VulkanMemoryManager::VulkanMemoryManager(VulkanDevice* pInDevice)
		: pDevice(pInDevice),
		  AllBufferAllocationsFreeListHead(-1)
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
		int32 OldestFrameToKeep = GFrameRenderThread - 3;
		uint32 NumAlloc = UBAllocations.PendingFree.Size();
		uint32 Index = 0;
		for (; Index < NumAlloc; ++Index)
		{
			UBPendingFree& Pending = UBAllocations.PendingFree[Index];
			if (Pending.Frame < OldestFrameToKeep)
			{
				FreeVulkanAllocation(Pending.Allocation);
			}
		}
	}

	void VulkanMemoryManager::FreeVulkanAllocation(VulkanAllocation& Allocation)
	{
		switch (Allocation.Type)
		{
		case EVulkanAllocationType::EAT_PooledBuffer:
			FreeVulkanAllocationPooledBuffer(Allocation);
			break;
		case EVulkanAllocationType::EAT_Buffer:
			FreeVulkanAllocationBuffer(Allocation);
			break;
		case EVulkanAllocationType::EAT_Image:
			FreeVulkanAllocationImage(Allocation);
			break;
		}
		WEngine::Memzero(&Allocation, sizeof(VulkanAllocation));
		Allocation.Type = EVulkanAllocationType::EAT_Empty;
		RE_ASSERT(!Allocation.HasAllocation(), "Failed to free allocation.");
	}

	void VulkanMemoryManager::FreeVulkanAllocationPooledBuffer(VulkanAllocation& Allocation)
	{
		uint32 Index = Allocation.AllocatorIndex;
		GetSubresourceAllocator(Index)->Free(Allocation);
	}

	void VulkanMemoryManager::FreeVulkanAllocationBuffer(VulkanAllocation& Allocation)
	{
		uint32 Index = Allocation.AllocatorIndex;
		GetSubresourceAllocator(Index)->Free(Allocation);
	}

	void VulkanMemoryManager::FreeVulkanAllocationImage(VulkanAllocation& Allocation)
	{
		uint32 Index = Allocation.AllocatorIndex;
		GetSubresourceAllocator(Index)->Free(Allocation);
	}

	void VulkanMemoryManager::AllocateUniformBuffer(VulkanAllocation& OutAllocation, uint32 Size, const void* Contents)
	{
		if (!AllocateBufferPooled(OutAllocation, Size, 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, EVulkanAllocationMetaType::EAMT_UniformBuffer))
		{
			return;
		}

		if (Contents)
		{
			memcpy(OutAllocation.GetMappedPointer(pDevice), Contents, Size);
			OutAllocation.FlushMappedMemory(pDevice);
		}
	}

	void VulkanMemoryManager::FreeUniformBuffer(VulkanAllocation& InAllocation)
	{
		if (InAllocation.HasAllocation())
		{
			WEngine::WScopeLock Lock(&UBAllocations.CS);
			ProcessPendingUBFrees();
			UBPendingFree& Pending = UBAllocations.PendingFree.AddInitialized();
			Pending.Frame = GFrameRenderThread;
			WEngine::Swap(&Pending.Allocation, &InAllocation);
			UBAllocations.Peak = WEngine::Max(UBAllocations.Peak, (uint32)UBAllocations.PendingFree.Size());
		}
	}

	void VulkanMemoryManager::RegisterSubresourceAllocator(VulkanSubresourceAllocator* Allocator)
	{
		WEngine::WRWScopeLock Lock(&AllBufferAllocatorsLock, RW_Write);
		if (AllBufferAllocationsFreeListHead != (PTRINT)-1)
		{
			const uint32 Index = (uint32)AllBufferAllocationsFreeListHead;
			AllBufferAllocationsFreeListHead = (PTRINT)AllBufferAllocators[Index];
			Allocator->AllocatorIndex = Index;
			AllBufferAllocators[Index] = Allocator;
		}
		else
		{
			Allocator->AllocatorIndex = AllBufferAllocators.Size();
			AllBufferAllocators.Push(Allocator);
		}
	}

	void VulkanMemoryManager::UnregisterSubresourceAllocator(VulkanSubresourceAllocator* Allocator)
	{
		WEngine::WRWScopeLock Lock(&AllBufferAllocatorsLock, RW_Write);
		const uint32 Index = Allocator->AllocatorIndex;
		AllBufferAllocators[Index] = (VulkanSubresourceAllocator*)AllBufferAllocationsFreeListHead;
		AllBufferAllocationsFreeListHead = (PTRINT)Index;
	}

	bool VulkanMemoryManager::ReleaseSubresourceAllocator(VulkanSubresourceAllocator* Allocator)
	{
		WEngine::WScopeLock Lock(&UsedFreeBufferSection);

		if (Allocator->JoinFreeBlocks())
		{
			if (Allocator->Type == EVulkanAllocationType::EAT_PooledBuffer)
			{
				UsedBufferAllocators[Allocator->PoolSizeIndex].RemoveSingleSwap(Allocator);
				Allocator->Frame = GFrameRenderThread;
				FreeBufferAllocators[Allocator->PoolSizeIndex].Push(Allocator);
			}
			else
			{
				
			}

			return true;
		}
		return false;
	}

	VulkanDeviceMemoryAllocation* VulkanMemoryManager::Alloc(uint32 MemoryTypeIndex, uint32 Size)
	{
		VkMemoryAllocateInfo MemoryAllocateInfo = {};
		{
			MemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			MemoryAllocateInfo.allocationSize = Size;
			MemoryAllocateInfo.memoryTypeIndex = MemoryTypeIndex;
		}
		VkDeviceMemory DeviceMemory;
		RE_ASSERT(vkAllocateMemory(pDevice->GetHandle(), &MemoryAllocateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &DeviceMemory) == VK_SUCCESS, "Failed to allocate memory.");

		VulkanDeviceMemoryAllocation *Allocation = new VulkanDeviceMemoryAllocation();
		Allocation->Device = pDevice->GetHandle();
		Allocation->Memory = DeviceMemory;
		Allocation->Size = Size;
		Allocation->MemoryTypeIndex = MemoryTypeIndex;
		Allocation->bCanBeMapped = VkEnumHasFlags(MemoryProperties.memoryTypes[MemoryTypeIndex].propertyFlags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		Allocation->bIsCached = VkEnumHasFlags(MemoryProperties.memoryTypes[MemoryTypeIndex].propertyFlags, VK_MEMORY_PROPERTY_HOST_CACHED_BIT);
		Allocation->bIsCoherent = VkEnumHasFlags(MemoryProperties.memoryTypes[MemoryTypeIndex].propertyFlags, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		return Allocation;
	}

	void VulkanMemoryManager::GetMemoryPropertyTypeIndex(uint32 MemoryPropertyBits, VkMemoryPropertyFlags MemoryPropertyFlags, uint32& OutTypeIndex)
	{
		for (int32 MemoryTypeIndex = 0; MemoryTypeIndex < MemoryProperties.memoryHeapCount; ++MemoryTypeIndex)
		{
			if ((MemoryPropertyBits & (0x01 << MemoryTypeIndex) != 0) && ((MemoryProperties.memoryTypes[MemoryTypeIndex].propertyFlags & MemoryPropertyFlags) == MemoryPropertyFlags))
			{
				OutTypeIndex = MemoryTypeIndex;
				return;
			}
		}
		RE_ASSERT(false, "No support memory heap.");
	}

	bool VulkanMemoryManager::AllocateBufferPooled(VulkanAllocation& OutAllocation, uint32 Size, uint32 MinAlignment, VkBufferUsageFlags UsageFlags, VkMemoryPropertyFlags MemoryPropertyFlags, EVulkanAllocationMetaType MetaType)
	{
		uint32 Alignment = WEngine::Max(MinAlignment, 0u);

		int32 PoolSize = (int32)GetPoolSize(Size, Alignment);
		if (PoolSize != (int32)EPoolSizes::SizeCount)
		{
			Size = PoolSizes[PoolSize];
		}

		WEngine::WScopeLock ScopeLock(&UsedFreeBufferSection);

		for (int32 Index = 0; Index < UsedBufferAllocators[PoolSize].Size(); ++Index)
		{
			VulkanSubresourceAllocator *SubresourceAllocator = UsedBufferAllocators[PoolSize][Index];
			if ((SubresourceAllocator->BufferUsageFlags & UsageFlags) == UsageFlags &&
				(SubresourceAllocator->MemoryPropertyFlags & MemoryPropertyFlags) == MemoryPropertyFlags)
			{
				if (SubresourceAllocator->TryAllocate(OutAllocation, Size, Alignment, MetaType))
				{
					return true;
				}
			}
		}

		for (int32 Index = 0; Index < FreeBufferAllocators[PoolSize].Size(); ++Index)
		{
			VulkanSubresourceAllocator *SubresourceAllocator = FreeBufferAllocators[PoolSize][Index];
			if ((SubresourceAllocator->BufferUsageFlags & UsageFlags) == UsageFlags &&
				(SubresourceAllocator->MemoryPropertyFlags & MemoryPropertyFlags) == MemoryPropertyFlags)
			{
				if (SubresourceAllocator->TryAllocate(OutAllocation, Size, Alignment, MetaType))
				{
					FreeBufferAllocators[PoolSize].RemoveAndSwap(Index);
					UsedBufferAllocators[PoolSize].Push(SubresourceAllocator);
					return true;
				}
			}
		}

		const uint32 BufferSize = WEngine::Max(Size, BufferSizes[PoolSize]);

		VkBuffer Buffer;
		VkBufferCreateInfo BufferCreateInfo = {};
		{
			BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			BufferCreateInfo.size = BufferSize;
			BufferCreateInfo.usage = UsageFlags;
		}
		RE_ASSERT(vkCreateBuffer(pDevice->GetHandle(), &BufferCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &Buffer) == VK_SUCCESS, "Failed to create buffer.");

		VkMemoryRequirements MemoryRequirements;
		vkGetBufferMemoryRequirements(pDevice->GetHandle(), Buffer, &MemoryRequirements);

		uint32 MemoryTypeIndex = 0;
		GetMemoryPropertyTypeIndex(MemoryRequirements.memoryTypeBits, MemoryPropertyFlags, MemoryTypeIndex);

		VulkanDeviceMemoryAllocation *DeviceMemoryAllocation = Alloc(MemoryTypeIndex, MemoryRequirements.size);

		vkBindBufferMemory(pDevice->GetHandle(), Buffer, DeviceMemoryAllocation->GetHandle(), 0);
		if (DeviceMemoryAllocation->CanBeMapped())
		{
			DeviceMemoryAllocation->Map(BufferSize, 0);
		}

		VulkanSubresourceAllocator *NewSubresourceAllocator = new VulkanSubresourceAllocator(EVulkanAllocationType::EAT_PooledBuffer, this, DeviceMemoryAllocation, BufferSize, MemoryRequirements.alignment, UsageFlags, MemoryPropertyFlags);

		RegisterSubresourceAllocator(NewSubresourceAllocator);
		UsedBufferAllocators[PoolSize].Push(NewSubresourceAllocator);

		if (NewSubresourceAllocator->TryAllocate(OutAllocation, Size, Alignment, MetaType))
		{
			return true;
		}

		return false;
	}

	bool VulkanMemoryManager::AllocateBufferMemory(VulkanAllocation& OutAllocation, const VkMemoryRequirements& MemoryRequirements, VkMemoryPropertyFlags MemoryPropertyFlags, EVulkanAllocationMetaType MetaType)
	{
		uint32 MemoryTypeIndex = 0;
		GetMemoryPropertyTypeIndex(MemoryRequirements.memoryTypeBits, MemoryPropertyFlags, MemoryTypeIndex);

		VulkanDeviceMemoryAllocation *DeviceMemoryAllocation = Alloc(MemoryTypeIndex, MemoryRequirements.size);

		VulkanSubresourceAllocator *SubresourceAllocator = new VulkanSubresourceAllocator(EVulkanAllocationType::EAT_Buffer, this, DeviceMemoryAllocation, MemoryRequirements.size, MemoryRequirements.alignment, )
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
	}

	VulkanStagingBuffer* VulkanStagingBufferManager::AcquireBuffer(uint32 InSize, VkBufferUsageFlags InUsageFlags, VkMemoryPropertyFlags InMemoryPropertyFlags)
	{
		for (uint32 Index = 0; Index < FreeStagingBuffers.Size(); ++Index)
		{
			FreeEntry& Entry = FreeStagingBuffers[Index];
			if (Entry.StagingBuffer->GetSize() == InSize && Entry.StagingBuffer->MemoryPropertyFlags == InMemoryPropertyFlags)
			{
				VulkanStagingBuffer *Buffer = Entry.StagingBuffer;
				FreeStagingBuffers.RemoveAndSwap(Index);
				UsedStagingBuffers.Push(Buffer);
				return Buffer;
			}
		}

		VkBufferCreateInfo BufferCreateInfo = {};
		{
			BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			BufferCreateInfo.size = InSize;
			BufferCreateInfo.usage = InUsageFlags;
		}

		VkBuffer Buffer;
		RE_ASSERT(vkCreateBuffer(pDevice->GetHandle(), &BufferCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &Buffer) == VK_SUCCESS, "Failed to create buffer.");

		VkMemoryRequirements MemoryRequirements;
		vkGetBufferMemoryRequirements(pDevice->GetHandle(), Buffer, &MemoryRequirements);
		MemoryRequirements.alignment = WEngine::Max((VkDeviceSize)16, MemoryRequirements.alignment);
		if (InMemoryPropertyFlags == VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
		{
			uint64 NonCoherentAtomSize = (uint64)pDevice->GetLimits().nonCoherentAtomSize;
			MemoryRequirements.alignment = WEngine::AlignArbitrary(MemoryRequirements.alignment, NonCoherentAtomSize);
		}

		int32 MemoryTypeIndex = 0;
		for (; MemoryTypeIndex < MemoryProperties.memoryTypeCount; ++MemoryTypeIndex)
		{
			if ((MemoryRequirements.memoryTypeBits & (0x01 << MemoryTypeIndex) != 0) && ((MemoryProperties.memoryTypes[MemoryTypeIndex].propertyFlags & InMemoryPropertyFlags) == InMemoryPropertyFlags))
			{
				break;
			}
		}
		RE_ASSERT(MemoryTypeIndex < MemoryProperties.memoryTypeCount, "No support memory heap.");

		VkMemoryAllocateInfo MemoryAllocateInfo = {};
		{
			MemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			MemoryAllocateInfo.memoryTypeIndex = MemoryTypeIndex;
			MemoryAllocateInfo.allocationSize = MemoryRequirements.size;
		}

		VkDeviceMemory Memory;
		vkAllocateMemory(pDevice->GetHandle(), &MemoryAllocateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &Memory);

		VulkanAllocation *Allocation = Alloc(Memory, MemoryRequirements.size);
		return new VulkanStagingBuffer(pDevice, Buffer, Allocation);
	}

	void VulkanStagingBufferManager::ReleaseBuffer(VulkanStagingBuffer* StagingBuffer)
	{
		FreeAllocations.Push(StagingBuffer->Allocation);
		delete StagingBuffer;
	}

}