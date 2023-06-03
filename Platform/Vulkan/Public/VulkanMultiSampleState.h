#pragma once

namespace Vulkan
{
	
	class VulkanMultiSampleState : public RHIMultiSampleState
	{
	public:

		VulkanMultiSampleState(const RHIMultiSampleStateInitializer& Initializer);

		virtual ~VulkanMultiSampleState() = default;

	private:

		VkPipelineMultisampleStateCreateInfo MultiSampleStateCreateInfo;

		friend class VulkanDevice;
		friend class VulkanPipelineStateManager;

	};

}