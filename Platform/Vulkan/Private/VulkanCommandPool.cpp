#include "pch.h"
#include "Platform/Vulkan/Public/VulkanCommandPool.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"

namespace Vulkan
{

	VulkanCommandPool::VulkanCommandPool(VkCommandPool* commandPool, VkDevice* device)
		: m_commandPool(commandPool), m_device(device)
	{
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
	}

	RHICommandBuffer* VulkanCommandPool::GetCommandBuffer(bool isPrimary)
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = *m_commandPool;
		commandBufferAllocateInfo.level = isPrimary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
		commandBufferAllocateInfo.commandBufferCount = 1;

		VkCommandBuffer* pCommandBuffer = (VkCommandBuffer*)WEngine::Allocator::Get()->Allocate(sizeof(VkCommandBuffer));
		::new (pCommandBuffer) VkCommandBuffer();

		VulkanCommandBuffer *commandBuffers = (VulkanCommandBuffer*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanCommandBuffer));
		vkAllocateCommandBuffers(*m_device, &commandBufferAllocateInfo, pCommandBuffer);
		::new (commandBuffers) VulkanCommandBuffer(pCommandBuffer, m_commandPool, !isPrimary, m_device);

		return commandBuffers;
	}

	std::vector<RHICommandBuffer*> VulkanCommandPool::GetCommandBuffer(unsigned int count, bool isPrimary)
	{
		std::vector<RHICommandBuffer*> commandBuffers(count);

		for(unsigned int i = 0; i < count; ++i)
		{
			VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
			commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			commandBufferAllocateInfo.commandPool = *m_commandPool;
			commandBufferAllocateInfo.level = isPrimary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
			commandBufferAllocateInfo.commandBufferCount = 1;

			VkCommandBuffer* pCommandBuffer = (VkCommandBuffer*)WEngine::Allocator::Get()->Allocate(sizeof(VkCommandBuffer));
			::new (pCommandBuffer) VkCommandBuffer();
			vkAllocateCommandBuffers(*m_device, &commandBufferAllocateInfo, pCommandBuffer);

			commandBuffers[i] = (VulkanCommandBuffer*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanCommandBuffer));
			::new (commandBuffers[i]) VulkanCommandBuffer(pCommandBuffer, m_commandPool, !isPrimary, m_device);
		}

		return commandBuffers;
	}

}