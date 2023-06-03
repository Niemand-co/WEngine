#pragma once

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

}