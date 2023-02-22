#include "pch.h"
#include "Platform/Vulkan/Encoder/Public/VulkanGraphicsEncoder.h"
#include "Platform/Vulkan/Public/VulkanHeads.h"
#include "Render/Descriptor/Public/RHIRenderPassBeginDescriptor.h"
#include "Render/Descriptor/Public/RHIBarrierDescriptor.h"
#include "RHI/Public/RHIScissor.h"
#include "RHI/Public/RHIViewport.h"
#include "Utils/Public/Window.h"
#include "Editor/Public/Screen.h"

namespace Vulkan
{

	VulkanGraphicsEncoder::VulkanGraphicsEncoder(VkCommandBuffer *cmd)
		: m_cmd(cmd)
	{
		m_width = 0;
		m_height = 0;
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
		renderPassBeginInfo.clearValueCount = descriptor->clearCount;
		
		WEngine::WArray<VkClearValue> values(descriptor->clearCount);
		for (unsigned int i = 0; i < descriptor->clearCount; ++i)
		{
			values[i] = { descriptor->pClearValues[i].Color[0], descriptor->pClearValues[i].Color[1], descriptor->pClearValues[i].Color[2], descriptor->pClearValues[i].Color[3] };
		}
		renderPassBeginInfo.pClearValues = values.GetData();

		vkCmdBeginRenderPass(*m_cmd, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		m_width = descriptor->renderTarget->GetWidth();
		m_height = descriptor->renderTarget->GetHeight();
	}

	void VulkanGraphicsEncoder::SetPipeline(RHIPipelineStateObject* pso)
	{
		vkCmdBindPipeline(*m_cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, *static_cast<VulkanPipelineStateObject*>(pso)->GetHandle());
	}

	void VulkanGraphicsEncoder::SetViewport(const RHIViewport& pViewport)
	{
		vkCmdSetViewport(*m_cmd, 0, 1, {});
	}

	void VulkanGraphicsEncoder::SetScissor(const RHIScissor& scissor)
	{
		vkCmdSetScissor(*m_cmd, 0, 1, ((const VulkanScissor*)&scissor)->GetHandle());
	}

	void VulkanGraphicsEncoder::SetLineWidth(float width)
	{
		vkCmdSetLineWidth(*m_cmd, width);
	}

	void VulkanGraphicsEncoder::SetDepthTestEnable(bool enable)
	{
		vkCmdSetDepthTestEnable(*m_cmd, enable);
	}

	void VulkanGraphicsEncoder::SetPolygonTopology(PrimitiveTopology primitive)
	{
		vkCmdSetPrimitiveTopology(*m_cmd, WEngine::ToVulkan(primitive));
	}

	void VulkanGraphicsEncoder::ClearRenderTarget(bool isClearColor, bool isClearDepth, glm::vec4 clearColor, float clearDepth, unsigned int clearStencil)
	{
		VkClearAttachment clearAttachment = {};
		{
			clearAttachment.aspectMask = (isClearColor ? VK_IMAGE_ASPECT_COLOR_BIT : 0) | (isClearDepth ? (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT) : 0);
			clearAttachment.colorAttachment = 0;
			clearAttachment.clearValue.color = { clearColor.x, clearColor.y, clearColor.z, clearColor.w };
			clearAttachment.clearValue.depthStencil = { clearDepth, clearStencil };
		}
		VkClearRect clearRect = {};
		{
			clearRect.rect = { (int)m_width, (int)m_height };
			clearRect.baseArrayLayer = 0;
			clearRect.layerCount = 1;
		}
		vkCmdClearAttachments(*m_cmd, 1, &clearAttachment, 1, &clearRect);
	}

	void VulkanGraphicsEncoder::BindVertexBuffer(RHIBuffer* pBuffer)
	{
		VkDeviceSize offets[] = {0};
		VkBuffer Buffer = static_cast<VulkanVertexBuffer*>(pBuffer)->GetHandle();
		vkCmdBindVertexBuffers(*m_cmd, 0, 1, &Buffer, offets);
	}

	void VulkanGraphicsEncoder::BindIndexBuffer(RHIBuffer* pBuffer)
	{
		vkCmdBindIndexBuffer(*m_cmd, static_cast<VulkanIndexBuffer*>(pBuffer)->GetHandle(), 0, VK_INDEX_TYPE_UINT32);
	}

	void VulkanGraphicsEncoder::BindGroups(unsigned int groupCount, RHIGroup* pGroup, RHIPipelineResourceLayout* pPipelineResourceLayout, unsigned int offsetCount, unsigned int *offsets)
	{
		vkCmdBindDescriptorSets(*m_cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, *static_cast<VulkanPipelineResourceLayout*>(pPipelineResourceLayout)->GetHandle(), 0, groupCount, static_cast<VulkanGroup*>(pGroup)->GetHandle(), offsetCount, offsets);
	}

	void VulkanGraphicsEncoder::DrawVertexArray()
	{
		vkCmdDraw(*m_cmd, 3, 1, 0, 0);
	}

	void VulkanGraphicsEncoder::DrawIndexed(unsigned int indexCount, unsigned int firstIndex, unsigned int instanceCount, unsigned int firstInstance)
	{
		vkCmdDrawIndexed(*m_cmd, indexCount, instanceCount, firstIndex, 0, firstInstance);
	}

	void VulkanGraphicsEncoder::ResourceBarrier(unsigned int srcStage, unsigned int dstStage)
	{
		vkCmdPipelineBarrier(*m_cmd, srcStage, dstStage, VkDependencyFlagBits(), 0, nullptr, 0, nullptr, 0, nullptr);
	}

	void VulkanGraphicsEncoder::ResourceBarrier(RHIBarrierDescriptor* pDescriptor)
	{

	}

	void VulkanGraphicsEncoder::CopyBufferToImage(RHITexture* pTexture, RHIBuffer* pBuffer, unsigned int width, unsigned int height)
	{
		VkBufferImageCopy bufferImageCopy = {};
		{
			bufferImageCopy.bufferOffset = 0;
			bufferImageCopy.bufferImageHeight = 0;
			bufferImageCopy.bufferRowLength = 0;

			bufferImageCopy.imageExtent = { width, height, 1 };
			bufferImageCopy.imageOffset = { 0, 0, 0 };

			bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			bufferImageCopy.imageSubresource.baseArrayLayer = 0;
			bufferImageCopy.imageSubresource.mipLevel = 0;
			bufferImageCopy.imageSubresource.layerCount = 1;
		}
		vkCmdCopyBufferToImage(*m_cmd, static_cast<VulkanTextureBuffer*>(pBuffer)->GetHandle(), static_cast<VulkanTexture2D*>(pTexture)->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);
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