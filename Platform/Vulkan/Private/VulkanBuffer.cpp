#include "pch.h"
#include "Platform/Vulkan/Public/VulkanBuffer.h"

namespace Vulkan
{

	VulkanBuffer::VulkanBuffer(VkBuffer* buffer, VkDevice *device, unsigned int memoryHeapIndex, size_t size, void* pData)
		: m_pBuffer(buffer), m_pDevice(device)
	{
		m_pMemoryRequirements = (VkMemoryRequirements*)WEngine::Allocator::Get()->Allocate(sizeof(VkMemoryRequirements));
		vkGetBufferMemoryRequirements(*m_pDevice, *m_pBuffer, m_pMemoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = m_pMemoryRequirements->size;
		memoryAllocateInfo.memoryTypeIndex = memoryHeapIndex;

		m_pDeviceMemory = (VkDeviceMemory*)WEngine::Allocator::Get()->Allocate(sizeof(VkDeviceMemory));
		RE_ASSERT(vkAllocateMemory(*m_pDevice, &memoryAllocateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), m_pDeviceMemory) == VK_SUCCESS, "Failed to Allocate Memory.");

		vkBindBufferMemory(*m_pDevice, *m_pBuffer, *m_pDeviceMemory, 0);

		RE_ASSERT(vkMapMemory(*m_pDevice, *m_pDeviceMemory, 0, size, 0, &m_pData) == VK_SUCCESS, "Failed to Map Memory To Host.");
		std::memcpy(m_pData, pData, size);
		vkUnmapMemory(*m_pDevice, *m_pDeviceMemory);
	}

	VulkanBuffer::~VulkanBuffer()
	{
		vkDestroyBuffer(*m_pDevice, *m_pBuffer, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks());
		vkFreeMemory(*m_pDevice, *m_pDeviceMemory, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks());
	}

	void VulkanBuffer::LoadData(void* pData, size_t size)
	{

	}

	VkBuffer* VulkanBuffer::GetHandle()
	{
		return m_pBuffer;
	}

}
