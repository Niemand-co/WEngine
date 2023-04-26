#include "pch.h"
#include "Platform/Vulkan/Public/VulkanPendingState.h"

namespace Vulkan
{

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

}