#include "pch.h"
#include "Platform/Vulkan/Encoder/Public/VulkanGraphicsEncoder.h"
#include "Platform/Vulkan/Public/VulkanHeads.h"
#include "Render/Descriptor/Public/RHIRenderPassBeginDescriptor.h"
#include "Utils/Public/Window.h"

namespace Vulkan
{

	VulkanGraphicsEncoder::VulkanGraphicsEncoder(VkCommandBuffer *cmd)
		: m_cmd(cmd)
	{
	}

	VulkanGraphicsEncoder::~VulkanGraphicsEncoder()
	{
	}

	void VulkanGraphicsEncoder::BeginPass(RHIRenderPassBeginDescriptor* descriptor)
	{
		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.framebuffer = *static_cast<VulkanRenderTarget*>(descriptor->renderTarget)->GetHandle();
		renderPassBeginInfo.renderPass = *static_cast<VulkanRenderPass*>(descriptor->renderPass)->GetHandle();
		renderPassBeginInfo.renderArea.offset = { 0, 0 };
		renderPassBeginInfo.renderArea.extent = { descriptor->renderTarget->GetWidth(), descriptor->renderTarget->GetHeight() };
		renderPassBeginInfo.clearValueCount = 1;
		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 0.0f}} };
		renderPassBeginInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(*m_cmd, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanGraphicsEncoder::SetPipeline(RHIPipelineStateObject* pso)
	{
		vkCmdBindPipeline(*m_cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, *static_cast<VulkanPipelineStateObject*>(pso)->GetHandle());
	}

	void VulkanGraphicsEncoder::SetViewport(RHIViewport* viewports)
	{
		VkViewport viewport = {};
		viewport.width = Window::cur_window->GetWidth();
		viewport.height = Window::cur_window->GetHeight();
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(*m_cmd, 0, 1, &viewport);
	}

	void VulkanGraphicsEncoder::SetScissor(RHIScissor* scissor)
	{
		VkRect2D rect = {};
		rect.offset = { 0, 0 };
		rect.extent = { Window::cur_window->GetWidth(), Window::cur_window->GetHeight() };
		vkCmdSetScissor(*m_cmd, 0, 1, &rect);
	}

	void VulkanGraphicsEncoder::ClearRenderTarget(bool isClearColor, bool isClearDepth, glm::vec4 clearColor, float clearDepth)
	{
		VkClearAttachment clearAttachment = {};
		{
			clearAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT | VK_IMAGE_ASPECT_DEPTH_BIT;
			clearAttachment.colorAttachment = 0;
			clearAttachment.clearValue.color = { clearColor.x, clearColor.y, clearColor.z, clearColor.w };
			clearAttachment.clearValue.depthStencil.depth = clearDepth;
		}
		VkClearRect clearRect = {};
		{
			clearRect.rect = { 1920, 1080 };
			clearRect.baseArrayLayer = 0;
			clearRect.layerCount = 1;
		}
		vkCmdClearAttachments(*m_cmd, 1, &clearAttachment, 1, &clearRect);
	}

	void VulkanGraphicsEncoder::BindVertexBuffer(RHIBuffer* pBuffer)
	{
		VkDeviceSize offets[] = {0};
		vkCmdBindVertexBuffers(*m_cmd, 0, 1, static_cast<VulkanBuffer*>(pBuffer)->GetHandle(), offets);
	}

	void VulkanGraphicsEncoder::BindIndexBuffer(RHIBuffer* pBuffer)
	{
		vkCmdBindIndexBuffer(*m_cmd, *static_cast<VulkanBuffer*>(pBuffer)->GetHandle(), 0, VK_INDEX_TYPE_UINT32);
	}

	void VulkanGraphicsEncoder::BindGroups(unsigned int groupCount, RHIGroup* pGroup, RHIPipelineResourceLayout* pPipelineResourceLayout)
	{
		vkCmdBindDescriptorSets(*m_cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, *static_cast<VulkanPipelineResourceLayout*>(pPipelineResourceLayout)->GetHandle(), 0, groupCount, static_cast<VulkanGroup*>(pGroup)->GetHandle(), 0, nullptr);
	}

	void VulkanGraphicsEncoder::DrawVertexArray()
	{
		vkCmdDraw(*m_cmd, 3, 1, 0, 0);
	}

	void VulkanGraphicsEncoder::DrawIndexed(unsigned int indexeCount, unsigned int firstIndex, unsigned int instanceCount, unsigned int firstInstance)
	{
		vkCmdDrawIndexed(*m_cmd, indexeCount, instanceCount, firstIndex, 0, firstInstance);
	}

	void VulkanGraphicsEncoder::ResourceBarrier(RHITexture *pTexture)
	{
		VkImageMemoryBarrier imageMemoryBarrier = {};
		{
			imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			imageMemoryBarrier.image = *static_cast<VulkanTexture*>(pTexture)->GetHandle();
			imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		}
		vkCmdPipelineBarrier(*m_cmd, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VkDependencyFlagBits(), 0, nullptr, 0, nullptr, 0, &imageMemoryBarrier);
	}

	void VulkanGraphicsEncoder::SetEvent(RHIEvent* pEvent)
	{
		vkCmdSetEvent(*m_cmd, *static_cast<VulkanEvent*>(pEvent)->GetHandle(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
	}

	void VulkanGraphicsEncoder::NextSubpass()
	{
		vkCmdNextSubpass(*m_cmd, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanGraphicsEncoder::EndPass()
	{	
		vkCmdEndRenderPass(*m_cmd);
	}

}