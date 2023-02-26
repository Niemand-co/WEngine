#include "pch.h"
#include "Platform/Vulkan/Public/VulkanCommandPool.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

namespace Vulkan
{

	VulkanCommandPool::VulkanCommandPool(VulkanDevice *pInDevice, VkCommandPoolCreateInfo *pInfo, VulkanCommandBufferManager *pInManager)
		: pDevice(pInDevice),
		  pManager(pInManager)
	{
		RE_ASSERT(vkCreateCommandPool(pDevice->GetHandle(), pInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &CommandPool) == VK_SUCCESS, "Failed to Get Command Pool.");
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		vkDestroyCommandPool(pDevice->GetHandle(), CommandPool, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	RHICommandBuffer* VulkanCommandPool::GetCommandBuffer()
	{
		VkCommandBufferAllocateInfo CommandBufferAllocateInfo = {};
		CommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		CommandBufferAllocateInfo.commandPool = CommandPool;
		CommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		CommandBufferAllocateInfo.commandBufferCount = 1;

		return new VulkanCommandBuffer(pDevice, this, &CommandBufferAllocateInfo);
	}

	VulkanCommandBuffer* VulkanCommandPool::AllocateCmdBuffer()
	{
		VkCommandBufferAllocateInfo CommandBufferAllocateInfo = {};
		CommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		CommandBufferAllocateInfo.commandPool = CommandPool;
		CommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		CommandBufferAllocateInfo.commandBufferCount = 1;

		VulkanCommandBuffer *CmdBuffer = new VulkanCommandBuffer(pDevice, this, &CommandBufferAllocateInfo);
		CmdBuffer->State = VulkanCommandBuffer::ECmdState::ReadyForBegin;

		CmdBuffers.Push(CmdBuffer);

		return CmdBuffer;
	}

}