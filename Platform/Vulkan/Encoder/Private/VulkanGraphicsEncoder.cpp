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
		std::vector<VkClearValue> values(descriptor->clearCount);
		for (unsigned int i = 0; i < descriptor->clearCount; ++i)
		{
			values[i].color = { descriptor->pClearValues[0].color.r, descriptor->pClearValues[0].color.g, descriptor->pClearValues[0].color.b, descriptor->pClearValues[0].color.a };
			values[i].depthStencil = { descriptor->pClearValues[i].depth, descriptor->pClearValues[i].stencil };
		}
		renderPassBeginInfo.pClearValues = values.data();

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
		VkViewport viewport = {};
		viewport.width = pViewport.width;
		viewport.height = pViewport.height;
		viewport.x = pViewport.posX;
		viewport.y = pViewport.posY;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(*m_cmd, 0, 1, &viewport);
	}

	void VulkanGraphicsEncoder::SetScissor(const RHIScissor& scissor)
	{
		VkRect2D rect = {};
		rect.offset = { scissor.offsetX, scissor.offsetY };
		rect.extent = { scissor.width, scissor.height };
		vkCmdSetScissor(*m_cmd, 0, 1, &rect);
	}

	void VulkanGraphicsEncoder::SetLineWidth(float width)
	{
		vkCmdSetLineWidth(*m_cmd, width);
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
		vkCmdBindVertexBuffers(*m_cmd, 0, 1, static_cast<VulkanBuffer*>(pBuffer)->GetHandle(), offets);
	}

	void VulkanGraphicsEncoder::BindIndexBuffer(RHIBuffer* pBuffer)
	{
		vkCmdBindIndexBuffer(*m_cmd, *static_cast<VulkanBuffer*>(pBuffer)->GetHandle(), 0, VK_INDEX_TYPE_UINT32);
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
		VkBufferMemoryBarrier *pBufferBarriers = (VkBufferMemoryBarrier*)WEngine::Allocator::Get()->Allocate(pDescriptor->bufferCount * sizeof(VkBufferMemoryBarrier));
		for (unsigned int i = 0; i < pDescriptor->bufferCount; ++i)
		{
			::new (pBufferBarriers + i) VkBufferMemoryBarrier();
			BufferBarrier *pBarrier = pDescriptor->pBufferBarriers + i;
			pBufferBarriers[i].sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
			pBufferBarriers[i].buffer = *static_cast<VulkanBuffer*>(pBarrier->pBuffer)->GetHandle();
			pBufferBarriers[i].size = pBarrier->pBuffer->Size() * pBarrier->pBuffer->Alignment();
			pBufferBarriers[i].offset = 0;
			pBufferBarriers[i].srcAccessMask = VK_ACCESS_HOST_READ_BIT;
			pBufferBarriers[i].dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
		}

		VkImageMemoryBarrier *pImageBarriers = (VkImageMemoryBarrier*)WEngine::Allocator::Get()->Allocate(pDescriptor->textureCount * sizeof(VkImageMemoryBarrier));
		for (unsigned int i = 0; i < pDescriptor->textureCount; ++i)
		{
			::new (pImageBarriers + i) VkImageMemoryBarrier();
			TextureBarrier *pBarrier = pDescriptor->pTextureBarriers + i;
			pImageBarriers[i].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			pImageBarriers[i].pNext = nullptr;
			pImageBarriers[i].image = *static_cast<VulkanTexture*>(pBarrier->pTexture)->GetHandle();
			pImageBarriers[i].oldLayout = WEngine::ToVulkan(pBarrier->oldLayout);
			pImageBarriers[i].newLayout = WEngine::ToVulkan(pBarrier->newLayout);
			pImageBarriers[i].srcAccessMask = pBarrier->srcAccess;
			pImageBarriers[i].dstAccessMask = pBarrier->dstAccess;
			pImageBarriers[i].subresourceRange.aspectMask = pBarrier->imageAspect;
			pImageBarriers[i].subresourceRange.baseArrayLayer = 0;
			pImageBarriers[i].subresourceRange.layerCount = 1;
			pImageBarriers[i].subresourceRange.baseMipLevel = 0;
			pImageBarriers[i].subresourceRange.levelCount = 1;
		}

		vkCmdPipelineBarrier(*m_cmd, pDescriptor->srcStage, pDescriptor->dstStage, 0, 0, nullptr, pDescriptor->bufferCount, pBufferBarriers, pDescriptor->textureCount, pImageBarriers);
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
		vkCmdCopyBufferToImage(*m_cmd, *static_cast<VulkanBuffer*>(pBuffer)->GetHandle(), *static_cast<VulkanTexture*>(pTexture)->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);
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