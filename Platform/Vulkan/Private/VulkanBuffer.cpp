#include "pch.h"
#include "Platform/Vulkan/Public/VulkanBuffer.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

namespace Vulkan
{

	VulkanBufferBase::VulkanBufferBase(VkBuffer* buffer, VkDevice *device, unsigned int memoryHeapIndex, bool isDynamic, VkDeviceSize size)
		: pBuffer(buffer), pDevice(device)
	{
		pMemoryRequirements = (VkMemoryRequirements*)WEngine::Allocator::Get()->Allocate(sizeof(VkMemoryRequirements));
		vkGetBufferMemoryRequirements(*device, *buffer, pMemoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = pMemoryRequirements->size;
		memoryAllocateInfo.memoryTypeIndex = memoryHeapIndex;

		pDeviceMemory = (VkDeviceMemory*)WEngine::Allocator::Get()->Allocate(sizeof(VkDeviceMemory));
		RE_ASSERT(vkAllocateMemory(*device, &memoryAllocateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pDeviceMemory) == VK_SUCCESS, "Failed to Allocate Memory.");

		vkBindBufferMemory(*pDevice, *pBuffer, *pDeviceMemory, 0);
	}

	VulkanBufferBase::VulkanBufferBase(VkBuffer* buffer, VkDevice* device, unsigned int memoryHeapIndex, bool isDynamic, size_t dynamicAlignment, size_t count)
		: pBuffer(buffer), pDevice(device)
	{
		pMemoryRequirements = (VkMemoryRequirements*)WEngine::Allocator::Get()->Allocate(sizeof(VkMemoryRequirements));
		vkGetBufferMemoryRequirements(*device, *buffer, pMemoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = pMemoryRequirements->size;
		memoryAllocateInfo.memoryTypeIndex = memoryHeapIndex;

		pDeviceMemory = (VkDeviceMemory*)WEngine::Allocator::Get()->Allocate(sizeof(VkDeviceMemory));
		RE_ASSERT(vkAllocateMemory(*device, &memoryAllocateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pDeviceMemory) == VK_SUCCESS, "Failed to Allocate Memory.");

		vkBindBufferMemory(*pDevice, *pBuffer, *pDeviceMemory, 0);
	}

	VulkanBufferBase::~VulkanBufferBase()
	{
		vkDestroyBuffer(*pDevice, *pBuffer, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks());
		vkFreeMemory(*pDevice, *pDeviceMemory, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks());
	}

	VkBuffer* VulkanBufferBase::GetHandle()
	{
		return pBuffer;
	}

	VulkanVertexBuffer::VulkanVertexBuffer(VulkanDevice* pInDevice, size_t inStride, size_t count)
	{
		RHIBufferDescriptor descriptor = {};
		{
			descriptor.count = count;
			descriptor.dataSize = count * inStride;
			descriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE;
		}
		pInDevice->CreateBuffer(&descriptor);
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
	}

}
