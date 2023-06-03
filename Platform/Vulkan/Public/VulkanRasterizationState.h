#pragma once

namespace Vulkan
{

	class VulkanRasterizationState : public RHIRasterizationState
	{
	public:

		VulkanRasterizationState(const RHIRasterizationStateInitializer& Initializer);

		virtual ~VulkanRasterizationState() = default;

	private:

		VkPipelineRasterizationStateCreateInfo RasterizationStateCreateInfo;

		VkPipelineInputAssemblyStateCreateInfo InputAssemblyStateCreateInfo;

		friend class VulkanDevice;
		friend class VulkanPipelineStateManager;

	};

}