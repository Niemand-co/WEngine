#pragma once

namespace Vulkan
{

	class VulkanPendingGfxState : public VulkanResource
	{
	public:

		VulkanPendingGfxState();

		virtual ~VulkanPendingGfxState();

		void Reset()
		{
			Viewports.SetNumZeroed(1);
			Scissors.SetNumZeroed(1);
		}

		bool SetGfxPipeline(class VulkanGraphicsPipelineState* InGfxPipeline, bool bForceReset);

		void PrepareForDraw(class VulkanCommandBuffer *CmdBuffer);

		void Bind(class VulkanCommandBuffer *CmdBuffer);

	protected:

		WEngine::WArray<VkViewport> Viewports;
		WEngine::WArray<VkRect2D> Scissors;

		float WireLineWidth = 1.0f;

		class VulkanGraphicsPipelineState* CurrentPipelineState;
		class VulkanGraphicsPipelineDescriptorState *CurrentState;
	};

	class VulkanPendingComputeState : public VulkanResource
	{

	};

}