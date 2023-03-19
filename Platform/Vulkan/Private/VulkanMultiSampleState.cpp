#include "pch.h"
#include "Platform/Vulkan/Public/VulkanMultiSampleState.h"

namespace Vulkan
{

	VulkanMultiSampleState::VulkanMultiSampleState(const RHIMultiSampleStateInitializer& Initializer)
	{
		MultiSampleStateCreateInfo = {};
		MultiSampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		MultiSampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
		MultiSampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		MultiSampleStateCreateInfo.minSampleShading = 1.0f;
		MultiSampleStateCreateInfo.pSampleMask = nullptr;
		MultiSampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
		MultiSampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

	}

}