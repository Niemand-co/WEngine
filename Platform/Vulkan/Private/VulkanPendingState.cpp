#include "pch.h"
#include "Platform/Vulkan/Public/VulkanPendingState.h"
#include "Platform/Vulkan/Public/VulkanPipelineStateObject.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Platform/Vulkan/Public/VulkanVertexInputState.h"
#include "Platform/Vulkan/Public/VulkanBuffer.h"

namespace Vulkan
{
	void VulkanPendingGfxState::SetViewport(float MinX, float MinY, float MinZ, float MaxX, float MaxY, float MaxZ)
	{
		Viewports.SetNumZeroed(1);

		Viewports[0].x = MinX;
		Viewports[0].y = MinY;
		Viewports[0].width = MaxX - MinX;
		Viewports[0].height = MaxY - MinY;
		Viewports[0].minDepth = MinZ;
		if (MinZ == MaxZ)
		{
			Viewports[0].maxDepth = MinZ + 1.0f;
		}
		else
		{
			Viewports[0].maxDepth = MaxZ;
		}

		SetScissorRect((uint32)MinX, (uint32)MinY, (uint32)(MaxX - MinX), (uint32)(MaxY - MinY));
		bScissorEnable = false;
	}

	void VulkanPendingGfxState::SetScissor(bool bInEnable, uint32 MinX, uint32 MinY, uint32 Width, uint32 Height)
	{
		if (bInEnable)
		{
			SetScissorRect(MinX, MinY, Width, Height);
		}
		else
		{
			SetScissorRect(Viewports[0].x, Viewports[0].y, Viewports[0].width, Viewports[0].height);
		}

		bScissorEnable = bInEnable;
	}

	void VulkanPendingGfxState::SetStreamSource(uint32 StreamIndex, VkBuffer Buffer, uint32 Offset)
	{
		PendingStreams[StreamIndex].Buffer = Buffer;
		PendingStreams[StreamIndex].Offset = Offset;
		bVertexStreamDirty = true;
	}

	void VulkanPendingGfxState::SetScissorRect(uint32 MinX, uint32 MinY, uint32 Width, uint32 Height)
	{
		Scissors.SetNumZeroed(1);

		Scissors[0].offset.x = MinX;
		Scissors[0].offset.y = MinY;
		Scissors[0].extent.width = Width;
		Scissors[0].extent.height = Height;
	}

	bool VulkanPendingGfxState::SetGfxPipeline(VulkanGraphicsPipelineState* InGfxPipeline, bool bForceReset)
	{
		bool bChanged = bForceReset;
		if (InGfxPipeline != CurrentPipelineState)
		{
			CurrentPipelineState = InGfxPipeline;

			bChanged = true;
		}

		if (bChanged)
		{
			Reset();
		}

		return bChanged;
	}

	void VulkanPendingGfxState::UpdateDynamicStates(VulkanCommandBuffer* CmdBuffer)
	{
		const bool bNeedsUpdateViewport = !CmdBuffer->bHasViewport || Viewports.Size() != CmdBuffer->CurrentViewports.Size() || memcmp(Viewports.GetData(), CmdBuffer->CurrentViewports.GetData(), sizeof(VkViewport) * Viewports.Size()) != 0;
		if (bNeedsUpdateViewport)
		{
			WEngine::WArray<VkViewport> FlipViewports = Viewports;
			for(VkViewport& FlipViewport : FlipViewports)
			{
				FlipViewport.y += FlipViewport.height;
				FlipViewport.height = -FlipViewport.height;
			}
			vkCmdSetViewport(CmdBuffer->GetHandle(), 0, FlipViewports.Size(), FlipViewports.GetData());

			CmdBuffer->CurrentViewports = Viewports;
			CmdBuffer->bHasViewport = true;
		}

		const bool bNeedsUpdateScissor = !CmdBuffer->bHasScissor || Scissors.Size() != CmdBuffer->CurrentScissors.Size() || memcmp(Scissors.GetData(), CmdBuffer->CurrentScissors.GetData(), sizeof(VkRect2D) * Scissors.Size());
		if (bNeedsUpdateScissor)
		{
			vkCmdSetScissor(CmdBuffer->GetHandle(), 0, Scissors.Size(), Scissors.GetData());
			CmdBuffer->CurrentScissors = Scissors;
			CmdBuffer->bHasScissor = true;
		}

		const bool bNeedsUpdateStencil = !CmdBuffer->bHasStencil || StencilReference != CmdBuffer->CurrentStencilReference;
		if (bNeedsUpdateStencil)
		{
			vkCmdSetStencilReference(CmdBuffer->GetHandle(), VK_STENCIL_FRONT_AND_BACK, StencilReference);
			CmdBuffer->CurrentStencilReference = StencilReference;
			CmdBuffer->bHasStencil = true;
		}

		CmdBuffer->bNeedsUpdateDynamicStates = false;
	}

	void VulkanPendingGfxState::PrepareForDraw(VulkanCommandBuffer* CmdBuffer)
	{
		if (CmdBuffer->bNeedsUpdateDynamicStates)
		{
			UpdateDynamicStates(CmdBuffer);
		}

		if(bVertexStreamDirty)
		{
			const VulkanVertexInputState *VertexInputeState = CurrentPipelineState->GetVertexInputState();
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

			for (uint32 StreamIndex = 0; StreamIndex < VertexInputeState->Bindings.Size(); ++StreamIndex)
			{
				VertexStream& Element = PendingStreams[StreamIndex];
				TemporalVB.Add(Element.Buffer, Element.Offset);
			}

			vkCmdBindVertexBuffers(CmdBuffer->GetHandle(), 0, TemporalVB.NumUsed, TemporalVB.VertexBuffers, TemporalVB.Offsets);
			bVertexStreamDirty = false;
		}
	}

	void VulkanPendingGfxState::Bind(VulkanCommandBuffer* CmdBuffer)
	{
		CurrentPipelineState->Bind(CmdBuffer);
	}

	void VulkanPendingGfxState::SetUniformBuffer()
	{
		
	}

}