#include "pch.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Platform/Vulkan/Encoder/Public/VulkanGraphicsEncoder.h"
#include "Platform/Vulkan/Encoder/Public/VulkanComputeEncoder.h"

namespace Vulkan
{

	VulkanCommandBuffer::VulkanCommandBuffer(VkCommandBuffer *commandBuffer)
		: m_commandBuffer(commandBuffer)
	{
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
	}

	void VulkanCommandBuffer::BeginScopePass(std::string passName)
	{
		VkCommandBufferBeginInfo commandbufferBeginInfo = {};
		commandbufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		RE_ASSERT(vkBeginCommandBuffer(*m_commandBuffer, &commandbufferBeginInfo) == VK_SUCCESS, "Failed to Begin Command Buffer.");
	}

	void VulkanCommandBuffer::EndScopePass()
	{
		vkEndCommandBuffer(*m_commandBuffer);
	}

	RHIGraphicsEncoder* VulkanCommandBuffer::GetGraphicsEncoder()
	{
		return new VulkanGraphicsEncoder(m_commandBuffer);
	}

	RHIComputeEncoder* VulkanCommandBuffer::GetComputeEncoder()
	{
		return nullptr;
	}

	VkCommandBuffer* VulkanCommandBuffer::GetHandle()
	{
		return m_commandBuffer;
	}

}