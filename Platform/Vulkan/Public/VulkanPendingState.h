#pragma once

namespace Vulkan
{

	class VulkanGraphicsPipelineDescriptorState : public VulkanResource
	{
	public:

		

	};

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

		void SetGfxPipeline(class VulkanGraphicsPipelineStateObject* InGfxPipeline, bool bForceReset);

	protected:

		WEngine::WArray<VkViewport> Viewports;
		WEngine::WArray<VkRect2D> Scissors;

		float WireLineWidth = 1.0f;

		VulkanGraphicsPipelineStateObject *CurrentPipelineState;

	};

	class VulkanPendingComputeState : public VulkanResource
	{

	};

}