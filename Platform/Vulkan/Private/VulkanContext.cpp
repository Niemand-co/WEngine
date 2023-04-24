#include "pch.h"
#include "Platform/Vulkan/Public/VulkanHeads.h"
#include "Render/Descriptor/Public/RHIBarrierDescriptor.h"
#include "Render/Descriptor/Public/RHIFramebufferDescriptor.h"
#include "Render/Mesh/Public/Vertex.h"

namespace Vulkan
{

	VulkanDynamicContext::VulkanDynamicContext()
	{
		pCommandBufferManager = new VulkanCommandBufferManager(static_cast<VulkanDevice*>(pDevice), static_cast<VulkanQueue*>(pQueue));
	}

	VulkanDynamicContext::~VulkanDynamicContext()
	{
	}

	void VulkanDynamicContext::RHIBeginDrawingViewport(RHIViewport* Viewport)
	{
		VulkanViewport *TrueViewport = dynamic_cast<VulkanViewport*>(Viewport);
		RE_ASSERT(TrueViewport != nullptr, "Error RHI.");
	}

	void VulkanDynamicContext::RHIEndDrawingViewport(RHIViewport* Viewport, bool bPresent)
	{
		if (bPresent)
		{
			VulkanCommandBuffer *ActiveCmdBuffer = pCommandBufferManager->GetActiveCommandBuffer();
			bool bSuccefullyPresent = Viewport->Present(ActiveCmdBuffer, pQueue);
		}
	}

	WRenderPassRHIRef VulkanDynamicContext::RHIBeginRenderPass(RHIRenderPassDescriptor* RenderPasDescriptor, RHIFramebufferDescriptor* FramebufferDescriptor)
	{
		VulkanRenderPass *Pass = static_cast<VulkanRenderPass*>(pDevice->GetOrCreateRenderPass(RenderPasDescriptor));
		VulkanFramebuffer *Framebuffer = static_cast<VulkanFramebuffer*>(pDevice->GetOrCreateFramebuffer(FramebufferDescriptor, Pass));

		VulkanCommandBuffer *CmdBuffer = pCommandBufferManager->GetActiveCommandBuffer();
		if (!CmdBuffer)
		{
			pCommandBufferManager->PrepareForNewActiveCmdBuffer();
			CmdBuffer = pCommandBufferManager->GetActiveCommandBuffer();
		}
		WEngine::WArray<VkClearValue> ClearValues(FramebufferDescriptor->AttachmentCount);
		ClearValues[0].color = { 1.0f, 0, 0, 0 };
		VkRenderPassBeginInfo Info = {};
		{
			Info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			Info.renderPass = Pass->GetHandle();
			Info.framebuffer = Framebuffer->GetHandle();
			Info.clearValueCount = ClearValues.Size();
			Info.pClearValues = ClearValues.GetData();
			Info.renderArea.offset = { 0, 0 };
			Info.renderArea.extent = { FramebufferDescriptor->extent.width, FramebufferDescriptor->extent.height };
		}
		vkCmdBeginRenderPass(CmdBuffer->GetHandle(), &Info, VK_SUBPASS_CONTENTS_INLINE);
		CmdBuffer->State = VulkanCommandBuffer::ECmdState::IsInsideRenderPass;
		return Pass;
	}

	void VulkanDynamicContext::RHIEndRenderPass()
	{
		VulkanCommandBuffer *CmdBuffer = pCommandBufferManager->GetActiveCommandBuffer();
		vkCmdEndRenderPass(CmdBuffer->GetHandle());
		CmdBuffer->State = VulkanCommandBuffer::ECmdState::IsInsideBegin;
	}

	void VulkanDynamicContext::RHISetViewport(float X, float Y, float Width, float Height, float MinDepth, float MaxDepth)
	{
		VulkanCommandBuffer* CmdBuffer = pCommandBufferManager->GetActiveCommandBuffer();
		VkViewport Viewport = { X, Y, Width, Height, MinDepth, MaxDepth };
		vkCmdSetViewport(CmdBuffer->GetHandle(), 0, 1, &Viewport);
	}

	void VulkanDynamicContext::RHISetScissor(int32 OffsetX, int32 OffsetY, uint32 Width, uint32 Height)
	{
		VulkanCommandBuffer* CmdBuffer = pCommandBufferManager->GetActiveCommandBuffer();
		VkRect2D Scissor = { OffsetX, OffsetY, Width, Height };
		vkCmdSetScissor(CmdBuffer->GetHandle(), 0, 1, &Scissor);
	}

	void VulkanDynamicContext::RHIBindVertexBuffer(WVertexFactory *InVertexFactory)
	{
	}

	void VulkanDynamicContext::RHISetStreamResource(const VertexInputStream& InStream)
	{
		struct WTemporalVB
		{
			VkBuffer VertexBuffers[MaxVertexInputElementCount];
			VkDeviceSize Offsets[MaxVertexInputElementCount];
			int32 NumUsed = 0;

			void Add(VkBuffer InBuffer, VkDeviceSize InOffset)
			{
				RE_ASSERT(NumUsed < MaxVertexInputElementCount, "Too many vertex input.");
				VertexBuffers[NumUsed] = InBuffer;
				Offsets[NumUsed] = InOffset;
				++NumUsed;
			}
		} TemporalVB;

		for (uint32 StreamIndex = 0; StreamIndex < InStream.Size(); ++StreamIndex)
		{
			const VertexInputStreamElement& Element = InStream[StreamIndex];
			TemporalVB.Add(static_cast<VulkanVertexBuffer*>(Element.VertexBuffer)->GetHandle(), Element.Offset);
		}

		VulkanCommandBuffer* CmdBuffer = pCommandBufferManager->GetActiveCommandBuffer();
		vkCmdBindVertexBuffers(CmdBuffer->GetHandle(), 0, TemporalVB.NumUsed, TemporalVB.VertexBuffers, TemporalVB.Offsets);
	}

	void VulkanDynamicContext::RHIBindIndexBuffer(WIndexBufferRHIRef InIndexBuffer)
	{
		VulkanCommandBuffer* CmdBuffer = pCommandBufferManager->GetActiveCommandBuffer();
		vkCmdBindIndexBuffer(CmdBuffer->GetHandle(), static_cast<VulkanIndexBuffer*>(InIndexBuffer)->GetHandle(), 0, VK_INDEX_TYPE_UINT32);
	}

	void VulkanDynamicContext::RHIDrawIndexedPrimitive(uint32 indexCount, uint32 firstIndex, uint32 instanceCount)
	{
		VulkanCommandBuffer *CmdBuffer = pCommandBufferManager->GetActiveCommandBuffer();
		vkCmdDrawIndexed(CmdBuffer->GetHandle(), indexCount, instanceCount, firstIndex, 0, 0);
		//vkCmdDraw(CmdBuffer->GetHandle(), 3, 1, 0, 0);
	}

	void VulkanDynamicContext::RHIBeginTransition(WEngine::WArray<RHIBarrierDescriptor>& Transitions)
	{
		VulkanPipelineBarrier PipelineBarrier;
		for (RHIBarrierDescriptor& Transition : Transitions)
		{

			VkPipelineStageFlags SrcPipelineStage, DstPipelineStage;
			VkAccessFlags SrcAccess, DstAccess;
			VkImageLayout SrcLayout, DstLayout;
			GetVkStageAndAccessFlags(Transition.AccessBefore, Transition.Texture->IsDepthFormat(), SrcPipelineStage, SrcAccess, SrcLayout);
			GetVkStageAndAccessFlags(Transition.AccessAfter, Transition.Texture->IsDepthFormat(), DstPipelineStage, DstAccess, DstLayout);

			if (Transition.Type == RHIBarrierDescriptor::EType::Texture)
			{
				VulkanTexture *Texture = static_cast<VulkanTexture*>(Transition.Texture);
				VkImageSubresourceRange Range = {};
				{
					Range.aspectMask = Texture->GetAspect(Texture->GetFormat());
					Range.baseArrayLayer = Transition.ArrayLayer;
					Range.layerCount = 1;
					Range.baseMipLevel = Transition.MipLevel;
					Range.levelCount = 1;
				}

				if(SrcLayout == DstLayout)
					PipelineBarrier.AddTransition(Texture->GetHandle(), Range, SrcAccess, DstAccess, SrcLayout);
				else
					PipelineBarrier.AddTransition(Texture->GetHandle(), Range, SrcAccess, DstAccess, SrcLayout, DstLayout);
			}
			else if (Transition.Type == RHIBarrierDescriptor::EType::Buffer)
			{
				VulkanBufferBase *BufferBase = VulkanBufferBase::Cast(Transition.Buffer);
				PipelineBarrier.AddTransition(BufferBase->GetHandle(), Transition.Range, Transition.Offset, SrcAccess, DstAccess);
			}
		}
		PipelineBarrier.Execute(static_cast<VulkanDynamicContext*>(GetDynamicRHI())->GetCmdBufferManager()->GetImmediateCommandBuffer());
	}

	void VulkanDynamicContext::CopyImageToBackBuffer(RHITexture* SrcTexture, RHITexture* DstTexture, int32 SrcSizeX, int32 SrcSizeY, int32 DstSizeX, int32 DstSizeY)
	{
		VulkanCommandBuffer *CmdBuffer = pCommandBufferManager->GetImmediateCommandBuffer();

		VkImage SrcImage = static_cast<VulkanTexture*>(SrcTexture)->GetHandle();
		VkImage DstImage = static_cast<VulkanTexture*>(DstTexture)->GetHandle();
		VkImageLayout SrcLayout = VulkanTextureLayoutManager::FindLayout(SrcImage);
		VkImageLayout DstLayout = VulkanTextureLayoutManager::FindLayout(DstImage);

		{
			VulkanPipelineBarrier Barrier;
			VkImageSubresourceRange Range = VulkanPipelineBarrier::GetTextureSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
			if(SrcLayout != VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
				Barrier.AddTransition(SrcImage, Range, SrcLayout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
			if(DstLayout != VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
				Barrier.AddTransition(DstImage, Range, DstLayout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
			Barrier.Execute(CmdBuffer);
		}

		if (SrcSizeX != DstSizeX || SrcSizeY != DstSizeY)
		{
			VkImageBlit Region = {};
			{
				Region.srcOffsets[0].x = 0;
				Region.srcOffsets[0].y = 0;
				Region.srcOffsets[0].z = 0;
				Region.srcOffsets[1].x = SrcSizeX;
				Region.srcOffsets[1].y = SrcSizeY;
				Region.srcOffsets[1].z = 1;
				Region.dstOffsets[0].x = 0;
				Region.dstOffsets[0].y = 0;
				Region.dstOffsets[0].z = 0;
				Region.dstOffsets[1].x = DstSizeX;
				Region.dstOffsets[1].y = DstSizeY;
				Region.dstOffsets[1].z = 1;
				Region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				Region.srcSubresource.layerCount = 1;
				Region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				Region.dstSubresource.layerCount = 1;
			}

			vkCmdBlitImage(CmdBuffer->GetHandle(), SrcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, DstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &Region, VK_FILTER_LINEAR);
		}
		else
		{
			VkImageCopy Region = {};
			{
				Region.extent.width = SrcSizeX;
				Region.extent.height = SrcSizeY;
				Region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				Region.srcSubresource.layerCount = 1;
				Region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				Region.dstSubresource.layerCount = 1;
			}

			vkCmdCopyImage(CmdBuffer->GetHandle(), SrcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, DstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &Region);
		}

		{
			VulkanPipelineBarrier Barrier;
			VkImageSubresourceRange Range = VulkanPipelineBarrier::GetTextureSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
			if (SrcLayout != VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
				Barrier.AddTransition(SrcImage, Range, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, SrcLayout);
			if (DstLayout != VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
				Barrier.AddTransition(DstImage, Range, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, DstLayout);
			Barrier.Execute(CmdBuffer);
		}
	}

	void VulkanDynamicContext::RHISetGraphicsPipelineState(RHIGraphicsPipelineStateDescriptor* descriptor)
	{
		WPsoRHIRef Pipeline = pDevice->GetOrCreateGraphicsPipelineState(descriptor);
		Pipeline->Bind(pCommandBufferManager->GetActiveCommandBuffer());
	}

	void VulkanDynamicContext::RHISetShaderUniformBuffer(RHIGraphicsShader* ShaderRHI, uint32 BufferIndex, WUniformBufferRHIRef UniformBuffer)
	{
		VulkanShaderBase *Shader = nullptr;
		VkShaderStageFlags ShaderStage = 0;
		switch (ShaderRHI->GetFrequency())
		{
		case EShaderFrequency::SF_Vertex:
			ShaderStage = VK_SHADER_STAGE_VERTEX_BIT;
			Shader = static_cast<VulkanVertexShader*>(ShaderRHI);
			break;
		case EShaderFrequency::SF_Geometry:
			ShaderStage = VK_SHADER_STAGE_GEOMETRY_BIT;
			Shader = static_cast<VulkanGeometryShader*>(ShaderRHI);
			break;
		case EShaderFrequency::SF_Pixel:
			ShaderStage = VK_SHADER_STAGE_FRAGMENT_BIT;
			Shader = static_cast<VulkanPixelShader*>(ShaderRHI);
			break;
		default:
			RE_ASSERT(false, "Undefined type of shader.");
			break;
		}

		
	}

	void VulkanDynamicContext::UpdateUniformBuffer(WUniformBufferRHIRef UniformBuffer, void* Contents)
	{
		VulkanCommandBuffer *ActiveCmdBuffer = pCommandBufferManager->GetActiveCommandBuffer();
		if (ActiveCmdBuffer && ActiveCmdBuffer->HasBegun())
		{
			pCommandBufferManager->SubmitActiveCommandBuffer();
			pCommandBufferManager->WaitForCommandBuffer(ActiveCmdBuffer);
		}
	}

	WBlendStateRHIRef VulkanStaticContext::CreateBlendState(const RHIBlendStateInitializer& Initializer)
	{
		return new VulkanBlendState(Initializer);
	}

	WDepthStencilStateRHIRef VulkanStaticContext::CreateDepthStencilState(const RHIDepthStencilStateInitializer& Initializer)
	{
		return new VulkanDepthStencilState(Initializer);
	}

	WRasterizationStateRHIRef VulkanStaticContext::CreateRasterizationState(const RHIRasterizationStateInitializer& Initializer)
	{
		return new VulkanRasterizationState(Initializer);
	}

	WMultiSampleStateRHIRef VulkanStaticContext::CreateMultiSampleState(const RHIMultiSampleStateInitializer& Initializer)
	{
		return new VulkanMultiSampleState(Initializer);
	}

	WVertexInputStateRHIRef VulkanStaticContext::GetOrCreateVertexInputState(const WEngine::WArray<VertexInputElement>& InElements)
	{
		uint32 VertexInputID = WEngine::MemCrc32(InElements.GetData(), sizeof(VertexInputElement) * InElements.Size());
		VulkanVertexInputState* VertexInput = VulkanVertexInputStateManager::GetVertexInput(VertexInputID);
		if (VertexInput)
		{
			return VertexInput;
		}
		VertexInput = new VulkanVertexInputState(InElements);
		VulkanVertexInputStateManager::AddVertexInput(VertexInputID, VertexInput);

		return VertexInput;
	}

}