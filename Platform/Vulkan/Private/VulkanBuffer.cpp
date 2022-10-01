#include "pch.h"
#include "Platform/Vulkan/Public/VulkanBuffer.h"

namespace Vulkan
{

	VulkanBuffer::VulkanBuffer(VkBuffer* buffer, VkDevice *device)
		: m_pBuffer(buffer), m_pDevice(device)
	{
		m_pMemoryRequiremtns = (VkMemoryRequirements*)WEngine::Allocator::Get()->Allocate(sizeof(VkMemoryRequirements));
		vkGetBufferMemoryRequirements(*m_pDevice, *m_pBuffer, m_pMemoryRequiremtns);

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = m_pMemoryRequiremtns->size;
		memoryAllocateInfo.memoryTypeIndex = 1;

		m_pDeviceMemory = (VkDeviceMemory*)WEngine::Allocator::Get()->Allocate(sizeof(VkDeviceMemory));
		RE_ASSERT(vkAllocateMemory(*m_pDevice, &memoryAllocateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), m_pDeviceMemory) == VK_SUCCESS, "Failed to Allocate Memory.");

		vkBindBufferMemory(*m_pDevice, *m_pBuffer, *m_pDeviceMemory, 0);

		void* data;
		vkMapMemory(*m_pDevice, *m_pDeviceMemory, 0, 3 * sizeof(float), 0, &data);
	}

	VulkanBuffer::~VulkanBuffer()
	{
		vkDestroyBuffer(*m_pDevice, *m_pBuffer, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks());
		vkFreeMemory(*m_pDevice, *m_pDeviceMemory, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks());
	}

	VkBuffer* VulkanBuffer::GetHandle()
	{
		return m_pBuffer;
	}

}
