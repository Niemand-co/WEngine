#pragma once
#include "RHI/Public/RHIBlendState.h"

namespace Vulkan
{
	
	class VulkanAttachmentBlendState : public RHIAttachmentBlendState
	{
	public:

		VulkanAttachmentBlendState(const RHIBlendStateInitializer& Initializer);

		virtual ~VulkanAttachmentBlendState() = default;

	private:

		VkPipelineColorBlendAttachmentState ColorBlendAttachmentState;

		friend class VulkanPipelineStateManager;

	};

	class VulkanBlendState : public RHIBlendState
	{
	public:

		VulkanBlendState() = default;

		virtual ~VulkanBlendState() = default;

		virtual void SetAttachmentBlendState(uint32, RHIAttachmentBlendState* InState) override;

	private:

		VulkanAttachmentBlendState* Attachments[MaxSimultaneousRenderTargets] = {0};

		friend class VulkanDevice;
		friend class VulkanPipelineStateManager;

	};

}