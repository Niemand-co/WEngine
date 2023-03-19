#pragma once
#include "RHI/Public/RHIBlendState.h"

namespace Vulkan
{

	class VulkanBlendState : public RHIBlendState
	{
	public:

		VulkanBlendState(const RHIBlendStateInitializer& Initializer);

		virtual ~VulkanBlendState() = default;

	private:

		VkPipelineColorBlendAttachmentState ColorBlendAttachmentState;

		friend class VulkanDevice;

	};

}