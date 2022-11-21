#include "pch.h"
#include "Platform/Vulkan/Public/VulkanBuffer.h"

namespace Vulkan
{

	VulkanBuffer::VulkanBuffer(VkBuffer* buffer, VkDevice *device, unsigned int memoryHeapIndex, bool isDynamic, VkDeviceSize size)
		: m_pBuffer(buffer), m_pDevice(device)
	{
		RHIBuffer::m_size = 1;
		RHIBuffer::m_dynamicAlignment = -1;
		RHIBuffer::m_isDynamic = isDynamic;

		m_pMemoryRequirements = (VkMemoryRequirements*)WEngine::Allocator::Get()->Allocate(sizeof(VkMemoryRequirements));
		vkGetBufferMemoryRequirements(*device, *buffer, m_pMemoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = m_pMemoryRequirements->size;
		memoryAllocateInfo.memoryTypeIndex = memoryHeapIndex;

		m_pDeviceMemory = (VkDeviceMemory*)WEngine::Allocator::Get()->Allocate(sizeof(VkDeviceMemory));
		RE_ASSERT(vkAllocateMemory(*device, &memoryAllocateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), m_pDeviceMemory) == VK_SUCCESS, "Failed to Allocate Memory.");

		vkBindBufferMemory(*m_pDevice, *m_pBuffer, *m_pDeviceMemory, 0);
	}

	VulkanBuffer::VulkanBuffer(VkBuffer* buffer, VkDevice* device, unsigned int memoryHeapIndex, bool isDynamic, size_t dynamicAlignment, size_t count)
		: m_pBuffer(buffer), m_pDevice(device)
	{
		RHIBuffer::m_capacity = count;
		RHIBuffer::m_dynamicAlignment = dynamicAlignment;
		RHIBuffer::m_isDynamic = isDynamic;

		m_pMemoryRequirements = (VkMemoryRequirements*)WEngine::Allocator::Get()->Allocate(sizeof(VkMemoryRequirements));
		vkGetBufferMemoryRequirements(*device, *buffer, m_pMemoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = m_pMemoryRequirements->size;
		memoryAllocateInfo.memoryTypeIndex = memoryHeapIndex;

		m_pDeviceMemory = (VkDeviceMemory*)WEngine::Allocator::Get()->Allocate(sizeof(VkDeviceMemory));
		RE_ASSERT(vkAllocateMemory(*device, &memoryAllocateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), m_pDeviceMemory) == VK_SUCCESS, "Failed to Allocate Memory.");

		vkBindBufferMemory(*m_pDevice, *m_pBuffer, *m_pDeviceMemory, 0);
	}

	VulkanBuffer::~VulkanBuffer()
	{
		vkDestroyBuffer(*m_pDevice, *m_pBuffer, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks());
		vkFreeMemory(*m_pDevice, *m_pDeviceMemory, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks());
	}

	void VulkanBuffer::LoadData(void* pData, size_t size, size_t offset)
	{
		if(m_isDynamic)
			RE_ASSERT(vkMapMemory(*m_pDevice, *m_pDeviceMemory, offset * m_dynamicAlignment, size, 0, &m_pData) == VK_SUCCESS, "Failed to Map Memory To Host.")
		else
			RE_ASSERT(vkMapMemory(*m_pDevice, *m_pDeviceMemory, offset, size, 0, &m_pData) == VK_SUCCESS, "Failed to Map Memory To Host.");
		::memcpy(m_pData, pData, static_cast<size_t>(size));
		vkUnmapMemory(*m_pDevice, *m_pDeviceMemory);
	}

	void VulkanBuffer::Flush(size_t range)
	{
		VkMappedMemoryRange memoryRange = {};
		{
			memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
			memoryRange.memory = *m_pDeviceMemory;
			memoryRange.offset = 0;
			if(m_isDynamic)
				memoryRange.size = m_size * m_dynamicAlignment;
			else
				memoryRange.size = range;
		}
		vkFlushMappedMemoryRanges(*m_pDevice, 1, &memoryRange);
	}

	void VulkanBuffer::Resize(size_t count)
	{
		if (count <= m_capacity)
		{
			m_size = count;
			return;
		}

	}

	VkBuffer* VulkanBuffer::GetHandle()
	{
		return m_pBuffer;
	}

}
