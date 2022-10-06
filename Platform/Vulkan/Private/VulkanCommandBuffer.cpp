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

	RHIGraphicsEncoder* VulkanCommandBuffer::GetGraphicsEncoder()
	{
		VulkanGraphicsEncoder *pEncoder = (VulkanGraphicsEncoder*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanGraphicsEncoder));
		::new (pEncoder) VulkanGraphicsEncoder(m_commandBuffer);
		return pEncoder;
	}

	RHIComputeEncoder* VulkanCommandBuffer::GetComputeEncoder()
	{
		return nullptr;
	}

	void VulkanCommandBuffer::Clear()
	{
		vkResetCommandBuffer(*m_commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
	}

	VkCommandBuffer* VulkanCommandBuffer::GetHandle()
	{
		return m_commandBuffer;
	}

}