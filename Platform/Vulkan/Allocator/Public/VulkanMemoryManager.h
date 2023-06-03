#pragma once

namespace Vulkan
{
	
	enum class EVulkanAllocationType : uint8
	{
		EAT_Empty,
		EAT_PooledBuffer,
		EAT_Buffer,
		EAT_Image,
	};

	enum class EVulkanAllocationMetaType : uint8
	{
		EAMT_Unknown,
		EAMT_UniformBuffer,
		EAMT_MultiBuffer,
		EAMT_RingBuffer,
		EAMT_FrameTempBuffer,
		EAMT_ImageRenderTarget,
		EAMT_ImageOther,
		EAMT_BufferUAV,
		EAMT_BufferStaging,
		EAMT_BufferOther,
		EAMT_Size,
	};

	enum
	{
		GPU_ONLY_HEAP_PAGE_SIZE = 128 * 1024 * 1024,
		STAGING_HEAP_PAGE_SIZE = 32 * 1024 * 1024,
	};

	struct VulkanPageSizeBucket
	{
		uint64 AllocationMax;
		uint32 PageSize;
		enum
		{
			BUCKET_MASK_IMAGE = 0x1,
			BUCKET_MASK_BUFFER = 0x2,
		};
		uint32 BucketMask;
	};
	
	class VulkanAllocation
	{
	public:

		VulkanAllocation() = default;

		virtual ~VulkanAllocation() = default;

		void Init(EVulkanAllocationType InType, EVulkanAllocationMetaType InMetaType, VkBuffer InBuffer, uint32 InSize, uint32 InOffset, uint32 InAllocatorIndex, uint32 InAllocationIndex);

		bool HasAllocation() const { return  Type != EVulkanAllocationType::EAT_Empty; }

		void* GetMappedPointer(class VulkanDevice *pDevice);

		void FlushMappedMemory(class VulkanDevice *pDevice);

		void InvalidateMappedMemory(class VulkanDevice *pDevice);

		void BindBuffer(VulkanDevice* Device, VkBuffer Buffer);

		void BindImage(VulkanDevice *Device, VkImage Image);

		uint32 GetSize() const { return Size; }

		VkBuffer GetBufferHandle() const { return Buffer; }

	private:

		uint32 Size = 0;

		uint32 Offset = 0;

		uint16 AllocatorIndex = 0;

		uint32 AllocationIndex = 0;

		EVulkanAllocationMetaType MetaType = EVulkanAllocationMetaType::EAMT_Unknown;

		EVulkanAllocationType Type;

		VkBuffer Buffer;

		friend class VulkanSubresourceAllocator;
		friend class VulkanMemoryManager;
		friend class VulkanStagingBufferManager;
	};

	class VulkanDeviceMemoryAllocation : public VulkanResource
	{
	public:

		VulkanDeviceMemoryAllocation() = default;

		virtual ~VulkanDeviceMemoryAllocation() = default;

		void* Map(uint32 MappingSize, uint32 MappingOffset);

		void Unmap();

		void FlushMappedMemory(VkDeviceSize FlushSize, VkDeviceSize FlushOffset);

		void InvalidateMappedMemory(VkDeviceSize InvalidateSize, VkDeviceSize InvalidateOffset);

		bool CanBeMapped() const { return bCanBeMapped; }

		bool IsCoherent() const { return bIsCoherent; }

		bool IsCached() const { return bIsCached; }

		uint32 GetSize() const { return Size; }

		void* GetMappedPointer() const { return MappedPointer; }

		VkDeviceMemory GetHandle() const { return Memory; }

	private:

		uint32 Size = 0;

		VkDevice Device = VK_NULL_HANDLE;

		VkDeviceMemory Memory = VK_NULL_HANDLE;

		void *MappedPointer = nullptr;

		uint32 MemoryTypeIndex = 0;

		uint8 bCanBeMapped : 1 = false;

		uint8 bIsCached : 1 = false;

		uint8 bIsCoherent : 1 = false;

		friend class VulkanMemoryManager;

	};

	class VulkanSubresourceAllocator : public VulkanResource
	{
	public:

		VulkanSubresourceAllocator(EVulkanAllocationType InType, VkBuffer InBuffer, class VulkanMemoryManager *InOwner, VulkanDeviceMemoryAllocation *InDeviceMemoryAllocation, uint32 InBufferSize, uint32 InAlignment, VkBufferUsageFlags InUsageFlags, VkMemoryPropertyFlags InMemoryPropertyFlags, uint32 InPoolSizeIndex);

		VulkanSubresourceAllocator(EVulkanAllocationType InType, class VulkanMemoryManager *InOwner, VulkanDeviceMemoryAllocation *InDeviceMemoryAllocation, uint32 InMemoryTypeIndex, uint32 InBucketId);

		virtual ~VulkanSubresourceAllocator();

		bool TryAllocate(VulkanAllocation &OutAllocation, uint32 InSize, uint32 InAlignment, EVulkanAllocationMetaType InMetaType);

		void Free(VulkanAllocation &Allocation);

		void* GetMappedPointer() const { return DeviceMemoryAllocation->GetMappedPointer(); }

		VulkanDeviceMemoryAllocation* GetMemoryHandle() const { return DeviceMemoryAllocation; }

		void FlushMappedMemory(VkDeviceSize Offset, VkDeviceSize Size) { DeviceMemoryAllocation->FlushMappedMemory(Size, Offset); }

		void InvalidateMappedMemory(VkDeviceSize Offset, VkDeviceSize Size) { DeviceMemoryAllocation->InvalidateMappedMemory(Size, Offset); }

		bool JoinFreeBlocks();

	private:

		EVulkanAllocationType Type;

		VkBuffer Buffer;

		uint16 AllocatorIndex;

		VulkanMemoryManager *Owner;

		VulkanDeviceMemoryAllocation *DeviceMemoryAllocation;

		uint32 BufferSize;

		uint32 Alignment;

		uint32 UsedSize;

		uint32 PoolSizeIndex;

		uint32 BucketId;

		uint32 MemoryTypeIndex;

		uint32 Frame;

		VkBufferUsageFlags BufferUsageFlags;

		VkMemoryPropertyFlags MemoryPropertyFlags;

		WCriticalSection Section;

		struct WRange
		{
			uint32 Size;
			uint32 Offset;

			static void AddAndMerge(WEngine::WArray<WRange>& List, WRange& Range)
			{
				for (WRange& R : List)
				{
					if (R.Offset + R.Size == Range.Offset)
					{
						R.Size += Range.Size;
						return;
					}
				}
				List.Push(Range);
			}
		};
		WEngine::WArray<WRange> FreeList;
		uint32 NumAllocations = 0;

		struct VulkanAllocationInternal
		{
			uint32 AllocationSize = 0;
			uint32 AllocationOffset = 0;
			uint32 Alignment = 0;
			int32 PreFree = -1;
		};
		WEngine::WArray<VulkanAllocationInternal> Internals;
		
		int32 AllocationFreeListHead;

		friend class VulkanMemoryManager;
		friend class VulkanResourceHeap;

	};

	class VulkanResourceHeap : public VulkanResource
	{
	public:

		VulkanResourceHeap(class VulkanMemoryManager *InOwner, uint32 InMemoryTypeIndex);

		virtual ~VulkanResourceHeap();

		bool TryAllocate(VulkanAllocation& OutAllocation, uint8 Type, uint32 Size, uint32 Alignment, EVulkanAllocationMetaType MetaType, bool bMapAllocation);

		void FreePage(VulkanSubresourceAllocator *Allocator);

		void ReleasePage(VulkanSubresourceAllocator *Allocator);

	private:

		uint32 GetPageSizeBucket(VulkanPageSizeBucket &Bucket, uint8 Type, uint32 AllocationSize);

	private:

		VulkanMemoryManager *Owner;

		uint32 MemoryTypeIndex;

		WEngine::WArray<VulkanPageSizeBucket> Buckets;

		WEngine::WArray<VulkanSubresourceAllocator*> Pages[5];

		WCriticalSection PageCS;

		friend class VulkanMemoryManager;

	};

	class VulkanMemoryManager : public VulkanResource
	{
	public:

		VulkanMemoryManager(class VulkanDevice *pInDevice);

		virtual ~VulkanMemoryManager();

		void Init();
		void ProcessPendingUBFrees();

		void FreeVulkanAllocation(VulkanAllocation &Allocation);
		void FreeVulkanAllocationPooledBuffer(VulkanAllocation& Allocation);
		void FreeVulkanAllocationBuffer(VulkanAllocation& Allocation);
		void FreeVulkanAllocationImage(VulkanAllocation& Allocation);

		void AllocateUniformBuffer(VulkanAllocation &OutAllocation, uint32 Size, const void *Contents);
		void FreeUniformBuffer(VulkanAllocation &InAllocation);

		void RegisterSubresourceAllocator(VulkanSubresourceAllocator *Allocator);
		void UnregisterSubresourceAllocator(VulkanSubresourceAllocator *Allocator);
		bool ReleaseSubresourceAllocator(VulkanSubresourceAllocator *Allocator);

		bool AllocateBufferPooled(VulkanAllocation &OutAllocation, uint32 Size, uint32 MinAlignment, VkBufferUsageFlags UsageFlags, VkMemoryPropertyFlags MemoryPropertyFlags, EVulkanAllocationMetaType MetaType);
		bool AllocateBufferMemory(VulkanAllocation &OutAllocation, const VkMemoryRequirements& MemoryRequirements, VkMemoryPropertyFlags MemoryPropertyFlags, EVulkanAllocationMetaType MetaType);
		bool AllocateImageMemory(VulkanAllocation &OutAllocation, const VkMemoryRequirements& MemoryRequirements, VkMemoryPropertyFlags MemoryPropertyFlags, EVulkanAllocationMetaType MetaType);

		VulkanSubresourceAllocator* GetSubresourceAllocator(uint16 AllocatorIndex)
		{
			return AllBufferAllocators[AllocatorIndex];
		}

		enum class EPoolSizes : uint16
		{
			E32,
			E64,
			E128,
			E256,
			E512,
			E1k,
			E2k,
			E4k,
			E8k,
			SizeCount
		};

		static constexpr uint32 PoolSizes[(int32)EPoolSizes::SizeCount + 1] = 
		{
			32,
			64,
			128,
			512,
			1024,
			2048,
			4096,
			8192,
		};

		static constexpr uint32 BufferSizes[(int32)EPoolSizes::SizeCount + 1] =
		{
			64 * 1024,
			64 * 1024,
			128 * 1024,
			128 * 1024,
			256 * 1024,
			256 * 1024,
			512 * 1024,
			512 * 1024,
			1024 * 1024,
			1024 * 1024,
		};

		VulkanDeviceMemoryAllocation* Alloc(uint32 MemoryTypeIndex, uint32 Size);

		void Free(VulkanDeviceMemoryAllocation* Allocation);

		bool GetMemoryPropertyTypeIndex(uint32 MemoryPropertyBits, VkMemoryPropertyFlags MemoryPropertyFlags, uint32& OutTypeIndex);

		EPoolSizes GetPoolSize(uint32 Size, uint32 Alignment)
		{
			EPoolSizes PoolSize = EPoolSizes::SizeCount;
			for (int32 i = 0; i < (int32)EPoolSizes::SizeCount; ++i)
			{
				if (PoolSizes[i] >= Size)
				{
					PoolSize = (EPoolSizes)i;
					return PoolSize;
				}
			}
			return PoolSize;
		}

	private:

		VulkanDevice *pDevice;

		VkPhysicalDeviceMemoryProperties MemoryProperties;

		WEngine::WArray<VulkanSubresourceAllocator*> UsedBufferAllocators[(int32)EPoolSizes::SizeCount + 1];

		WEngine::WArray<VulkanSubresourceAllocator*> FreeBufferAllocators[(int32)EPoolSizes::SizeCount + 1];

		WEngine::WArray<VulkanSubresourceAllocator*> AllBufferAllocators;

		PTRINT AllBufferAllocationsFreeListHead;

		WRWLock AllBufferAllocatorsLock;

		WCriticalSection DeviceMemoryCS;

		WCriticalSection UsedFreeBufferSection;

		struct UBPendingFree
		{
			VulkanAllocation Allocation;
			uint64 Frame = 0;
		};

		struct
		{
			WCriticalSection CS;
			WEngine::WArray<UBPendingFree> PendingFree;
			uint32 Peak = 0;
		} UBAllocations;

		WEngine::WArray<VulkanResourceHeap*> ResourceTypeHeaps;

	};

	class VulkanStagingBufferManager : public VulkanResource
	{
	public:

		VulkanStagingBufferManager(VulkanDevice *pInDevice);

		virtual ~VulkanStagingBufferManager();

		void Tick();

		class VulkanStagingBuffer* AcquireBuffer(uint32 InSize, VkBufferUsageFlags InUsageFlags, VkMemoryPropertyFlags InMemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void ReleaseBuffer(VulkanStagingBuffer *StagingBuffer);

	private:

		VulkanDevice *pDevice;

		VkPhysicalDeviceMemoryProperties MemoryProperties;

		struct FreeEntry
		{
			VulkanStagingBuffer *StagingBuffer;
			uint32 Frame;
		};

		WEngine::WArray<VulkanStagingBuffer*> UsedStagingBuffers;

		WEngine::WArray<FreeEntry> FreeStagingBuffers;

		WCriticalSection StagingBufferCS;

	};

}

template<> struct WEngine::TIsElementTypePOD<Vulkan::VulkanSubresourceAllocator::VulkanAllocationInternal> { enum { Value = true }; };