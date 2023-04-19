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
	
	class VulkanAllocation : public RHIResource
	{
	public:

		VulkanAllocation() = default;

		virtual ~VulkanAllocation() = default;

		void Init(EVulkanAllocationType InType, EVulkanAllocationMetaType InMetaType, uint32 InSize, uint32 InOffset, uint32 InAllocatorIndex, uint32 InAllocationIndex);

		bool HasAllocation() const { return  Type != EVulkanAllocationType::EAT_Empty; }

		void* GetMappedPointer(class VulkanDevice *pDevice);

		void FlushMappedMemory(class VulkanDevice *pDevice);

		void InvalidateMappedMemory(class VulkanDevice *pDevice);

		uint32 GetSize() const { return Size; }

	private:

		uint32 Size = 0;

		uint32 Offset = 0;

		uint16 AllocatorIndex = 0;

		uint32 AllocationIndex = 0;

		EVulkanAllocationMetaType MetaType = EVulkanAllocationMetaType::EAMT_Unknown;

		EVulkanAllocationType Type;

		friend class VulkanSubresourceAllocator;
		friend class VulkanMemoryManager;
		friend class VulkanStagingBufferManager;
	};

	class VulkanDeviceMemoryAllocation : public RHIResource
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

	class VulkanSubresourceAllocator : public RHIResource
	{
	public:

		VulkanSubresourceAllocator(EVulkanAllocationType InType, class VulkanMemoryManager *InOwner, VulkanDeviceMemoryAllocation *InDeviceMemoryAllocation, uint32 InBufferSize, uint32 InAlignment, VkBufferUsageFlags InUsageFlags, VkMemoryPropertyFlags InMemoryPropertyFlags);

		virtual ~VulkanSubresourceAllocator();

		bool TryAllocate(VulkanAllocation &OutAllocation, uint32 InSize, uint32 InAlignment, EVulkanAllocationMetaType InMetaType);

		void* GetMappedPointer() const { return DeviceMemoryAllocation->GetMappedPointer(); }

		void FlushMappedMemory(VkDeviceSize Offset, VkDeviceSize Size) { DeviceMemoryAllocation->FlushMappedMemory(Size, Offset); }

		void InvalidateMappedMemory(VkDeviceSize Offset, VkDeviceSize Size) { DeviceMemoryAllocation->InvalidateMappedMemory(Size, Offset); }

	private:

		EVulkanAllocationType Type;

		uint16 AllocatorIndex;

		VulkanMemoryManager *Owner;

		VulkanDeviceMemoryAllocation *DeviceMemoryAllocation;

		uint32 BufferSize;

		uint32 Alignment;

		uint32 UsedSize;

		VkBufferUsageFlags BufferUsageFlags;

		VkMemoryPropertyFlags MemoryPropertyFlags;

		WCriticalSection Section;

		struct WRange
		{
			uint32 Size;
			uint32 Offset;
		};
		WEngine::WArray<WRange> FreeList;
		uint32 NumAllocations = 0;

		friend class VulkanMemoryManager;

	};

	class VulkanPendingFree : public RHIResource
	{

	};

	class VulkanMemoryManager : public RHIResource
	{
	public:

		VulkanMemoryManager(class VulkanDevice *pInDevice);

		virtual ~VulkanMemoryManager();

		void Init();

		void ProcessPendingUBFrees();

		void AllocateImage(VkImage& InOutImage, VkImageUsageFlags& InUsageFlags);

		void AllocateUniformBuffer(VulkanAllocation &OutAllocation, uint32 Size, const void *Contents);

		void FreeUniformBuffer(VulkanAllocation &InAllocation);

		void DeallocateBuffer(VkBuffer& InBuffer, VulkanAllocation*& InAllocation);

		void RegisterSubresourceAllocator(VulkanSubresourceAllocator *Allocator);

		void UnregisterSubresourceAllocator(VulkanSubresourceAllocator *Allocator);

		VulkanSubresourceAllocator* GetSubresourceAllocator(uint16 AllocatorIndex)
		{
			return AllBufferAllocators[AllocatorIndex];
		}

	private:

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

		bool AllocateBufferPooled(VulkanAllocation &OutAllocation, uint32 Size, uint32 MinAlignment, VkBufferUsageFlags UsageFlags, VkMemoryPropertyFlags MemoryPropertyFlags, EVulkanAllocationMetaType MetaType);

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

		WEngine::WArray<VulkanAllocation*> UsedAllocations;

		WEngine::WArray<VulkanAllocation*> FreeAllocations;

		WEngine::WArray<VulkanSubresourceAllocator*> UsedBufferAllocators[(int32)EPoolSizes::SizeCount + 1];

		WEngine::WArray<VulkanSubresourceAllocator*> FreeBufferAllocators[(int32)EPoolSizes::SizeCount + 1];

		WEngine::WArray<VulkanSubresourceAllocator*> AllBufferAllocators;

		PTRINT AllBufferAllocationsFreeListHead;

		WRWLock AllBufferAllocatorsLock;

		WCriticalSection UsedBufferSection;

		WCriticalSection FreeBufferSection;

	};

	class VulkanStagingBufferManager : public RHIResource
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
			uint32 NumFrames;
		};

		WEngine::WArray<VulkanStagingBuffer*> UsedStagingBuffers;

		WEngine::WArray<FreeEntry> FreeStagingBuffers;

	};

}