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

	void VulkanAllocation::BindBuffer(VulkanDevice *Device, VkBuffer Buffer)
	{
		VkDeviceMemory DeviceMemory;
		{
			VulkanSubresourceAllocator *Allocator = Device->GetMemoryManager()->GetSubresourceAllocator(AllocatorIndex);
			DeviceMemory = Allocator->GetMemoryHandle()->GetHandle();
		}
		RE_ASSERT(vkBindBufferMemory(Device->GetHandle(), Buffer, DeviceMemory, Offset) == VK_SUCCESS, "Failed to bind buffer with memory.");
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
		  PoolSizeIndex(InPoolSizeIndex),
		  AllocationFreeListHead(-1)
	{
		WRange FullRange(BufferSize, 0);
		FreeList.Push(FullRange);
	}

	VulkanSubresourceAllocator::VulkanSubresourceAllocator(EVulkanAllocationType InType, VulkanMemoryManager* InOwner, VulkanDeviceMemoryAllocation* InDeviceMemoryAllocation, uint32 InMemoryTypeIndex, uint32 InBucketId)
		: Type(InType),
		  Owner(InOwner),
		  DeviceMemoryAllocation(InDeviceMemoryAllocation),
		  BufferSize(InDeviceMemoryAllocation->GetSize()),
		  Alignment(0),
		  UsedSize(0),
		  Frame(0),
		  BufferUsageFlags(0),
		  MemoryPropertyFlags(0),
		  PoolSizeIndex(0x7fffffff),
		  BucketId(InBucketId),
		  MemoryTypeIndex(InMemoryTypeIndex),
		  AllocationFreeListHead(-1)
	{
		WRange FullRange(BufferSize, 0);
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

				if (AllocationFreeListHead)
				{
					OutAllocation.Init(Type, InMetaType, InSize, AlignedOffset, AllocatorIndex, NumAllocations);
					VulkanAllocationInternal& Data = Internals.AddInitialized();
					Data.Alignment = InAlignment;
					Data.AllocationSize = AllocateSize;
					Data.AllocationOffset = AllocatedOffset;
				}
				else
				{
					OutAllocation.Init(Type, InMetaType, InSize, AlignedOffset, AllocatorIndex, AllocationFreeListHead);
					VulkanAllocationInternal& Data = Internals[AllocationFreeListHead];
					AllocationFreeListHead = Data.PreFree;
					Data.Alignment = InAlignment;
					Data.AllocationSize = AllocateSize;
					Data.AllocationOffset = AllocatedOffset;
					Data.PreFree = -1;
				}
				UsedSize += AllocateSize;
				NumAllocations++;
			}
			return true;
		}

		return false;
	}

	void VulkanSubresourceAllocator::Free(VulkanAllocation& Allocation)
	{
		bool bCanFree = false;
		{
			WEngine::WScopeLock Lock(&Section);
			VulkanAllocationInternal& Data = Internals[Allocation.AllocationIndex];
			uint32 AllocationOffset = Data.AllocationOffset;
			uint32 AllocationSize = Data.AllocationSize;
			WRange Range = {};
			{
				Range.Size = AllocationSize;
				Range.Offset = AllocationOffset;
			}

			Data.PreFree = AllocationFreeListHead;
			AllocationFreeListHead = Allocation.AllocationIndex;

			WRange::AddAndMerge(FreeList, Range);
			UsedSize -= AllocationSize;
			NumAllocations--;
			if (JoinFreeBlocks())
			{
				bCanFree = true;
			}
		}

		if (bCanFree)
		{
			Owner->ReleaseSubresourceAllocator(this);
		}
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

	bool VulkanResourceHeap::TryAllocate(VulkanAllocation& OutAllocation, uint8 Type, uint32 Size, uint32 Alignment, EVulkanAllocationMetaType MetaType, bool bMapAllocation)
	{
		WEngine::WScopeLock Lock(&PageCS);
		VulkanPageSizeBucket Bucket;
		uint32 BucketId = GetPageSizeBucket(Bucket, Type, Size);
		WEngine::WArray<VulkanSubresourceAllocator*>& UsedPages = Pages[BucketId];

		for (uint32 Index = 0; Index < UsedPages.Size(); ++Index)
		{
			VulkanSubresourceAllocator* Page = UsedPages[Index];
			if (Page->TryAllocate(OutAllocation, Size, Alignment, MetaType))
			{
				return true;
			}
		}

		uint32 AllocationSize = WEngine::Max(Size, Bucket.PageSize);

		VulkanDeviceMemoryAllocation *DeviceMemoryAllocation = Owner->Alloc(MemoryTypeIndex, AllocationSize);
		if (!DeviceMemoryAllocation && AllocationSize != Size)
		{
			DeviceMemoryAllocation = Owner->Alloc(MemoryTypeIndex, Size);
			if (!DeviceMemoryAllocation)
			{
				return false;
			}
		}

		if (bMapAllocation)
		{
			DeviceMemoryAllocation->Map(AllocationSize, 0);
		}
		VulkanSubresourceAllocator *NewPage = new VulkanSubresourceAllocator(Type == 0x1 ? EVulkanAllocationType::EAT_Buffer : EVulkanAllocationType::EAT_Image, Owner, DeviceMemoryAllocation, MemoryTypeIndex, BucketId);
		Owner->RegisterSubresourceAllocator(NewPage);
		UsedPages.Push(NewPage);
		
		if (NewPage->TryAllocate(OutAllocation, Size, Alignment, MetaType))
		{
			return true;
		}

		return false;
	}

	void VulkanResourceHeap::FreePage(VulkanSubresourceAllocator *Allocator)
	{
		RE_ASSERT(Allocator->JoinFreeBlocks(), "Cannot free an allocator still hold allocation(s).");
		Allocator->Frame = GFrameRenderThread;

		uint32 BucketId = Allocator->BucketId;
		uint32 Index = Pages[BucketId].FindIndex(Allocator);
		Pages[BucketId].RemoveAndSwap(Index);

		ReleasePage(Allocator);
	}

	void VulkanResourceHeap::ReleasePage(VulkanSubresourceAllocator* Allocator)
	{
		Owner->UnregisterSubresourceAllocator(Allocator);
		VulkanDeviceMemoryAllocation *DeviceMemoryAllocation = Allocator->DeviceMemoryAllocation;
		Allocator->DeviceMemoryAllocation = nullptr;
		Owner->Free(Allocator->DeviceMemoryAllocation);
		delete Allocator;
	}

	uint32 VulkanResourceHeap::GetPageSizeBucket(VulkanPageSizeBucket& OutBucket, uint8 Type, uint32 AllocationSize)
	{
		for (uint32 BucketIndex = 0; BucketIndex < Buckets.Size(); ++BucketIndex)
		{
			VulkanPageSizeBucket& Bucket = Buckets[BucketIndex];
			if ((Bucket.BucketMask & Type) == Type && AllocationSize <= Bucket.AllocationMax)
			{
				OutBucket = Bucket;
				return BucketIndex;
			}
		}
		return 0xffffffff;
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
		const uint32 MemoryPropertyBits = (1 << MemoryProperties.memoryTypeCount) - 1;
		ResourceTypeHeaps.AddZero(MemoryProperties.memoryTypeCount);

		{
			uint32 TypeIndex = 0;
			GetMemoryPropertyTypeIndex(MemoryPropertyBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, TypeIndex);
			ResourceTypeHeaps[TypeIndex] = new VulkanResourceHeap(this, TypeIndex);

			WEngine::WArray<VulkanPageSizeBucket>& Buckets = ResourceTypeHeaps[TypeIndex]->Buckets;
			Buckets.Push({ STAGING_HEAP_PAGE_SIZE, STAGING_HEAP_PAGE_SIZE, VulkanPageSizeBucket::BUCKET_MASK_IMAGE | VulkanPageSizeBucket::BUCKET_MASK_BUFFER });
		}

		{
			uint32 TypeIndex = 0;
			{
				if (GetMemoryPropertyTypeIndex(MemoryPropertyBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, TypeIndex))
				{
				}
				else if (GetMemoryPropertyTypeIndex(MemoryPropertyBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, TypeIndex))
				{
				}
			}
			ResourceTypeHeaps[TypeIndex] = new VulkanResourceHeap(this, TypeIndex);

			WEngine::WArray<VulkanPageSizeBucket>& Buckets = ResourceTypeHeaps[TypeIndex]->Buckets;
			Buckets.Push({ STAGING_HEAP_PAGE_SIZE, STAGING_HEAP_PAGE_SIZE, VulkanPageSizeBucket::BUCKET_MASK_IMAGE | VulkanPageSizeBucket::BUCKET_MASK_BUFFER });
		}

		{
			uint32 TypeIndex = 0;
			RE_ASSERT(GetMemoryPropertyTypeIndex(MemoryPropertyBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, TypeIndex), "The device does not support GPU memory.");

			for (; TypeIndex < MemoryProperties.memoryTypeCount; ++TypeIndex)
			{
				const uint32 HeapIndex = MemoryProperties.memoryTypes[TypeIndex].heapIndex;
				const VkDeviceSize HeapSize = MemoryProperties.memoryHeaps[HeapIndex].size;

				if (!ResourceTypeHeaps[TypeIndex])
				{
					ResourceTypeHeaps[TypeIndex] = new VulkanResourceHeap(this, TypeIndex);

					WEngine::WArray<VulkanPageSizeBucket>& Buckets = ResourceTypeHeaps[TypeIndex]->Buckets;
					uint32 SmallAllocationThreshold = 2llu << 20llu;
					uint32 LargeAllocationThreshold = 64llu << 20llu;
					VkDeviceSize SmallPageSize = 8llu << 20llu;
					VkDeviceSize LargePageSize = WEngine::Min<VkDeviceSize>(HeapSize / 8, GPU_ONLY_HEAP_PAGE_SIZE);

					Buckets.Push({ SmallAllocationThreshold, (uint32)SmallPageSize, VulkanPageSizeBucket::BUCKET_MASK_BUFFER });
					Buckets.Push({ LargeAllocationThreshold, (uint32)LargePageSize, VulkanPageSizeBucket::BUCKET_MASK_BUFFER });
					Buckets.Push({ SmallAllocationThreshold, (uint32)SmallPageSize, VulkanPageSizeBucket::BUCKET_MASK_IMAGE });
					Buckets.Push({ LargeAllocationThreshold, (uint32)LargePageSize, VulkanPageSizeBucket::BUCKET_MASK_IMAGE });
				}
			}
		}
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
				VulkanResourceHeap *Heap = ResourceTypeHeaps[Allocator->MemoryTypeIndex];
				Heap->FreePage(Allocator);
			}

			return true;
		}
		return false;
	}

	VulkanDeviceMemoryAllocation* VulkanMemoryManager::Alloc(uint32 MemoryTypeIndex, uint32 Size)
	{
		WEngine::WScopeLock Lock(&DeviceMemoryCS);
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

	bool VulkanMemoryManager::GetMemoryPropertyTypeIndex(uint32 MemoryPropertyBits, VkMemoryPropertyFlags MemoryPropertyFlags, uint32& OutTypeIndex)
	{
		for (int32 MemoryTypeIndex = 0; MemoryTypeIndex < MemoryProperties.memoryHeapCount; ++MemoryTypeIndex)
		{
			if ((MemoryPropertyBits & (0x01 << MemoryTypeIndex) != 0) && ((MemoryProperties.memoryTypes[MemoryTypeIndex].propertyFlags & MemoryPropertyFlags) == MemoryPropertyFlags))
			{
				OutTypeIndex = MemoryTypeIndex;
				return true;
			}
		}
		return false;
	}

	void VulkanMemoryManager::Free(VulkanDeviceMemoryAllocation* Allocation)
	{
		WEngine::WScopeLock Lock(&DeviceMemoryCS);
		vkFreeMemory(pDevice->GetHandle(), Allocation->Memory, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());

		delete Allocation;
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

		VulkanSubresourceAllocator *NewSubresourceAllocator = new VulkanSubresourceAllocator(EVulkanAllocationType::EAT_PooledBuffer, this, DeviceMemoryAllocation, BufferSize, MemoryRequirements.alignment, UsageFlags, MemoryPropertyFlags, PoolSize);

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
		bool bShouldMap = VkEnumHasFlags(MemoryPropertyFlags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

		if (ResourceTypeHeaps[MemoryTypeIndex]->TryAllocate(OutAllocation, 0x1, MemoryRequirements.size, MemoryRequirements.alignment, MetaType, bShouldMap))
		{
			return true;
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
	}

	VulkanStagingBuffer* VulkanStagingBufferManager::AcquireBuffer(uint32 InSize, VkBufferUsageFlags InUsageFlags, VkMemoryPropertyFlags InMemoryPropertyFlags)
	{
		if (InMemoryPropertyFlags == VK_MEMORY_PROPERTY_HOST_CACHED_BIT)
		{
			uint64 NonCoherentAtomSize = (uint64)pDevice->GetLimits().nonCoherentAtomSize;
			InSize = WEngine::AlignArbitrary(InSize, NonCoherentAtomSize);
		}

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

		VulkanStagingBuffer *StagingBuffer = new VulkanStagingBuffer(pDevice);

		VkBufferCreateInfo BufferCreateInfo = {};
		{
			BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			BufferCreateInfo.size = InSize;
			BufferCreateInfo.usage = InUsageFlags;
		}
		RE_ASSERT(vkCreateBuffer(pDevice->GetHandle(), &BufferCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &StagingBuffer->Buffer) == VK_SUCCESS, "Failed to create buffer.");

		VkMemoryRequirements MemoryRequirements;
		vkGetBufferMemoryRequirements(pDevice->GetHandle(), StagingBuffer->Buffer, &MemoryRequirements);
		MemoryRequirements.alignment = WEngine::Max((VkDeviceSize)16, MemoryRequirements.alignment);
		if (InMemoryPropertyFlags == VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
		{
			uint64 NonCoherentAtomSize = (uint64)pDevice->GetLimits().nonCoherentAtomSize;
			MemoryRequirements.alignment = WEngine::AlignArbitrary(MemoryRequirements.alignment, NonCoherentAtomSize);
		}

		RE_ASSERT(pDevice->GetMemoryManager()->AllocateBufferMemory(StagingBuffer->Allocation, MemoryRequirements, InMemoryPropertyFlags | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, EVulkanAllocationMetaType::EAMT_BufferStaging), "Failed to create staging buffer allocation.");
		StagingBuffer->Size = InSize;
		StagingBuffer->Allocation.BindBuffer(pDevice, StagingBuffer->Buffer);

		{
			WEngine::WScopeLock Lock(&StagingBufferCS);
			UsedStagingBuffers.Push(StagingBuffer);
		}
	}

	void VulkanStagingBufferManager::ReleaseBuffer(VulkanStagingBuffer* StagingBuffer)
	{
		FreeStagingBuffers.Push({ StagingBuffer, GFrameRenderThread });
		delete StagingBuffer;
	}

}