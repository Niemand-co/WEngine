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
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = *m_commandPool;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocateInfo.commandBufferCount = num;

		//VkCommandBuffer* commandBuffer = (VkCommandBuffer*)WEngine::Allocator::Get()->Allocate(num * sizeof(VkCommandBuffer));
		VkCommandBuffer *commandBuffer = (VkCommandBuffer*)malloc(num * sizeof(VkCommandBuffer));
		vkAllocateCommandBuffers(*m_device, &commandBufferAllocateInfo, commandBuffer);

		return new VulkanCommandBuffer(commandBuffer);
	}

}