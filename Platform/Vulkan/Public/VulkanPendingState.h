#pragma once

namespace Vulkan
{

	class VulkanPendingGfxState : public VulkanResource
	{
	public:

		VulkanPendingGfxState()
		{
			Reset();
		}

		virtual ~VulkanPendingGfxState() = default;

		void Reset()
		{
			Viewports.SetNumZeroed(1);
			Scissors.SetNumZeroed(1);
			bScissorEnable = false;
			bVertexStreamDirty = true;
		}

		void SetViewport(float MinX, float MinY, float MinZ, float MaxX, float MaxY, float MaxZ);

		void SetScissor(bool bInEnable, uint32 MinX, uint32 MinY, uint32 Width, uint32 Height);

		void SetStreamSource(uint32 StreamIndex, VkBuffer Buffer, uint32 Offset);

		void SetScissorRect(uint32 MinX, uint32 MinY, uint32 Width, uint32 Height);

		bool SetGfxPipeline(class VulkanGraphicsPipelineState* InGfxPipeline, bool bForceReset);

		inline void SetStencilReference(uint32 InStencilReference)
		{
			if (InStencilReference != StencilReference)
			{
				StencilReference = InStencilReference;
			}
		}

		void UpdateDynamicStates(class VulkanCommandBuffer *CmdBuffer);

		void PrepareForDraw(class VulkanCommandBuffer *CmdBuffer);

		void Bind(class VulkanCommandBuffer *CmdBuffer);

		void SetUniformBuffer();

	protected:

		WEngine::WArray<VkViewport> Viewports;
		WEngine::WArray<VkRect2D> Scissors;
		bool bScissorEnable;

		float WireLineWidth = 1.0f;
		uint32 StencilReference;

		struct VertexStream
		{
			VkBuffer Buffer;
			uint32 Offset;
		};
		VertexStream PendingStreams[MaxVertexInputElementCount];
		uint8 bVertexStreamDirty : 1;

		class VulkanGraphicsPipelineState* CurrentPipelineState;
		class VulkanGraphicsPipelineDescriptorState *CurrentState;
	};

	class VulkanPendingComputeState : public VulkanResource
	{

	};

}