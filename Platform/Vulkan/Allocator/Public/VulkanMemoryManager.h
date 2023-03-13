#pragma once

namespace Vulkan
{
	
	class VulkanAllocation : public RHIResource
	{
	public:

		VulkanAllocation(class VulkanDevice *pInDevice, VkDeviceMemory& InMempry, uint32 Size);

		~VulkanAllocation();

		void BindBuffer(VkBuffer& InBuffer);

		VkDeviceMemory GetMemoryHandle() const { return Memory; }

		uint32 GetSize() const { return Size; }

	private:

		VulkanDevice *pDevice;

		VkDeviceMemory Memory;

		uint32 Size;

		uint32 FramesAfterLastUsed;

		friend class VulkanMemoryManager;
		friend class VulkanStagingBufferManager;
	};

	class VulkanMemoryManager : public RHIResource
	{
	public:

		VulkanMemoryManager(class VulkanDevice *pInDevice);

		virtual ~VulkanMemoryManager();

		void Init();

		void Tick();

		void AllocateBuffer(VkBuffer& InOutBuffer, VulkanAllocation*& InOutAllocation, VkBufferUsageFlags& InUsageFlags, VkMemoryPropertyFlags& MemoryPropertyFlags);

		void AllocateImage(VkImage& InOutImage, VkImageUsageFlags& InUsageFlags);

		void DeallocateBuffer(VkBuffer& InBuffer, VulkanAllocation*& InAllocation);

	private:

		VulkanAllocation* Alloc(VkDeviceMemory& Memory, uint32 InSize);

	private:

		VulkanDevice *pDevice;

		VkPhysicalDeviceMemoryProperties MemoryProperties;

		WEngine::WArray<VulkanAllocation*> UsedAllocations;

		WEngine::WArray<VulkanAllocation*> FreeAllocations;

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