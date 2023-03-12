#pragma once

namespace Vulkan
{
	
	class VulkanAllocation : public RHIResource
	{
	public:

		VulkanAllocation(class VulkanDevice *pInDevice, VkDeviceMemory& InMempry);

		~VulkanAllocation();

		void BindBuffer(VkBuffer& InBuffer);

		VkDeviceMemory GetMemoryHandle() const { return Memory; }

	private:

		VulkanDevice *pDevice;

		VkDeviceMemory Memory;

		uint32 FramesAfterLastUsed;

		friend class VulkanMemoryManager;
	};

	class VulkanMemoryManager : public RHIResource
	{
	public:

		VulkanMemoryManager(class VulkanDevice *pInDevice);

		~VulkanMemoryManager();

		void Init();

		void Tick();

		void AllocateBuffer(VkBuffer& InOutBuffer, VulkanAllocation*& InOutAllocation, VkBufferUsageFlags& InUsageFlags, VkMemoryPropertyFlags& MemoryPropertyFlags);

		void AllocateImage(VkImage& InOutImage, VkImageUsageFlags& InUsageFlags);

		void DeallocateBuffer(VkBuffer& InBuffer, VulkanAllocation*& InAllocation);

	private:

		VulkanAllocation* Alloc(VkDeviceMemory& Memory);

	private:

		VulkanDevice *pDevice;

		VkPhysicalDeviceMemoryProperties MemoryProperties;

		WEngine::WArray<VulkanAllocation*> UsedAllocations;

		WEngine::WArray<VulkanAllocation*> FreeAllocations;

	};

}