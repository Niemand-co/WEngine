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

	RHICommandBuffer* VulkanCommandPool::GetCommandBuffer(unsigned int num)
	{
		RHICommandBuffer *commandBuffers;

		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = *m_commandPool;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocateInfo.commandBufferCount = num;

		VkCommandBuffer* pCommandBuffer = (VkCommandBuffer*)WEngine::Allocator::Get()->Allocate(num * sizeof(VkCommandBuffer));

		commandBuffers = (RHICommandBuffer*)WEngine::Allocator::Get()->Allocate(num * sizeof(VulkanCommandBuffer));
		for (unsigned int i = 0; i < num; ++i)
		{
			vkAllocateCommandBuffers(*m_device, &commandBufferAllocateInfo, pCommandBuffer + i);
			::new (commandBuffers + i) VulkanCommandBuffer(pCommandBuffer + i, m_commandPool, m_device);
		}

		return commandBuffers;
	}

}