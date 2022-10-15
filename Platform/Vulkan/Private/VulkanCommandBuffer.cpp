#include "pch.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Platform/Vulkan/Encoder/Public/VulkanGraphicsEncoder.h"
#include "Platform/Vulkan/Encoder/Public/VulkanComputeEncoder.h"

namespace Vulkan
{

	VulkanCommandBuffer::VulkanCommandBuffer(VkCommandBuffer *commandBuffer, VkCommandPool *pCommandPool, VkDevice *pDevice)
		: m_commandBuffer(commandBuffer), m_pCommandPool(pCommandPool), m_pDevice(pDevice)
	{
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		vkFreeCommandBuffers(*m_pDevice, *m_pCommandPool, 1, m_commandBuffer);
	}

	void VulkanCommandBuffer::BeginScopePass(std::string passName)
	{
		VkCommandBufferBeginInfo commandbufferBeginInfo = {};
		commandbufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandbufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		RE_ASSERT(vkBeginCommandBuffer(*m_commandBuffer, &commandbufferBeginInfo) == VK_SUCCESS, "Failed to Begin Command Buffer.");
	}

	void VulkanCommandBuffer::EndScopePass()
	{
		vkEndCommandBuffer(*m_commandBuffer);
	}

	void VulkanCommandBuffer::ExecuteCommandBuffer(RHICommandBuffer* pCommandBuffer)
	{
		vkCmdExecuteCommands(*m_commandBuffer, 1, static_cast<VulkanCommandBuffer*>(pCommandBuffer)->GetHandle());
	}

	RHIGraphicsEncoder* VulkanCommandBuffer::GetGraphicsEncoder()
	{
		return new VulkanGraphicsEncoder(m_commandBuffer);
	}

	RHIComputeEncoder* VulkanCommandBuffer::GetComputeEncoder()
	{
		return nullptr;
	}

	void VulkanCommandBuffer::Clear()
	{
		vkResetCommandBuffer(*m_commandBuffer, 0);
	}

	VkCommandBuffer* VulkanCommandBuffer::GetHandle()
	{
		return m_commandBuffer;
	}

}