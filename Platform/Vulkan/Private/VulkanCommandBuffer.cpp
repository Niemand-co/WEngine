#include "pch.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Platform/Vulkan/Public/VulkanRenderPass.h"
#include "Platform/Vulkan/Public/VulkanRenderTarget.h"
#include "Platform/Vulkan/Encoder/Public/VulkanGraphicsEncoder.h"
#include "Platform/Vulkan/Encoder/Public/VulkanComputeEncoder.h"

namespace Vulkan
{

	VulkanCommandBuffer::VulkanCommandBuffer(VkCommandBuffer *commandBuffer, VkCommandPool *pCommandPool, bool isSecondary, VkDevice *pDevice)
		: m_commandBuffer(commandBuffer), m_pCommandPool(pCommandPool), m_isSecondary(isSecondary), m_pDevice(pDevice)
	{
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		vkFreeCommandBuffers(*m_pDevice, *m_pCommandPool, 1, m_commandBuffer);
	}

	void VulkanCommandBuffer::BeginScopePass(std::string passName, RHIRenderPass* pRenderPass, unsigned int subpass, RHIRenderTarget* pRenderTarget)
	{
		VkCommandBufferBeginInfo commandbufferBeginInfo = {};
		commandbufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandbufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		VkCommandBufferInheritanceInfo inheritanceInfo = {};

		if (m_isSecondary)
		{
			inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
			if(pRenderPass != nullptr)
				inheritanceInfo.renderPass = *static_cast<VulkanRenderPass*>(pRenderPass)->GetHandle();
			inheritanceInfo.subpass = subpass;
			if(pRenderTarget != nullptr)
				inheritanceInfo.framebuffer = *static_cast<VulkanRenderTarget*>(pRenderTarget)->GetHandle();
			commandbufferBeginInfo.pInheritanceInfo = &inheritanceInfo;
		}

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