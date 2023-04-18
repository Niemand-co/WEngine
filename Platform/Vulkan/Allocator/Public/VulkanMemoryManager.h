#pragma once

namespace Vulkan
{
	
	enum class EVulkanAllocationType : uint8
	{
		EAT_PooledBuffer,
		EAT_Buffer,
		EAT_Image,
	};
	
	class VulkanAllocation : public RHIResource
	{
	public:

		VulkanAllocation() = default;

		VulkanAllocation(class VulkanDevice *pInDevice, VkDeviceMemory& InMemory, uint32 INSize);

		virtual ~VulkanAllocation();

		void Init(class VulkanDevice* pInDevice, VkDeviceMemory& InMemory, uint32 InSize);

		void BindBuffer(VkBuffer& InBuffer);

		VkDeviceMemory GetMemoryHandle() const { return Memory; }

		uint32 GetSize() const { return Size; }

	private:

		VulkanDevice *pDevice = nullptr;

		VkDeviceMemory Memory;

		uint32 Size = 0;

		uint32 FramesAfterLastUsed = 0;

		friend class VulkanSubresourceAllocation;
		friend class VulkanMemoryManager;
		friend class VulkanStagingBufferManager;
	};

	class VulkanSubresourceAllocation : public RHIResource
	{
	public:

		VulkanSubresourceAllocation(EVulkanAllocationType InType, class VulkanMemoryManager *InOwner, uint32 InUsedSize, uint32 InBufferId, VkBufferUsageFlags InUsageFlags, VkMemoryPropertyFlags InMemoryPropertyFlags);

		virtual ~VulkanSubresourceAllocation();

	private:

		EVulkanAllocationType Type;

		VulkanMemoryManager *Owner;

		VulkanAllocation *Allocation;

		uint32 UsedSize;

		uint32 BufferId;

		VkBufferUsageFlags BufferUsageFlags;

		VkMemoryPropertyFlags MemoryPropertyFlags;

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

		void AllocateBuffer(VkBuffer& InOutBuffer, VulkanAllocation*& InOutAllocation, uint32 InSize, VkBufferUsageFlags& InUsageFlags, VkMemoryPropertyFlags& MemoryPropertyFlags);

		void AllocateImage(VkImage& InOutImage, VkImageUsageFlags& InUsageFlags);

		void AllocateUniformBuffer(VulkanAllocation &OutAllocation, uint32 Size, const void *Contents);

		void DeallocateBuffer(VkBuffer& InBuffer, VulkanAllocation*& InAllocation);

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

		VulkanAllocation* Alloc(VkDeviceMemory& Memory, uint32 InSize);

		bool AllocateBufferPooled(VulkanAllocation &OutAllocation, uint32 Size, uint32 MinAlignment, VkBufferUsageFlags UsageFlags, VkMemoryPropertyFlags MemortPropertyFlags);

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

		WEngine::WArray<VulkanSubresourceAllocation*> UsedBufferAllocations[(int32)EPoolSizes::SizeCount + 1];

		WEngine::WArray<VulkanSubresourceAllocation*> FreeBufferAllocations[(int32)EPoolSizes::SizeCount + 1];

		WCriticalSection UsedBufferSection;

		WCriticalSection FreeBufferSection;

	};

	class VulkanStagingBufferManager : public RHIResource
	{
	public:

		VulkanStagingBufferManager(VulkanDevice *pInDevice);

		virtual ~VulkanStagingBufferManager();

		void Tick();

		class VulkanStagingBuffer* AcquireBuffer(uint32 Size, VkBufferUsageFlags UsageFlags, VkMemoryPropertyFlags MemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void ReleaseBuffer(VulkanStagingBuffer *StagingBuffer);

	private:

		VulkanAllocation* Alloc(VkDeviceMemory& Memory, uint32 InSize);

	private:

		VulkanDevice *pDevice;

		VkPhysicalDeviceMemoryProperties MemoryProperties;

		WEngine::WArray<VulkanAllocation*> UsedAllocations;

		WEngine::WArray<VulkanAllocation*> FreeAllocations;

	};

}