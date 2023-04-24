#include "pch.h"
#include "Platform/Vulkan/Public/VulkanPendingState.h"

namespace Vulkan
{

	void VulkanPendingGfxState::SetGfxPipeline(VulkanGraphicsPipelineStateObject* InGfxPipeline, bool bForceReset)
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
	}

}