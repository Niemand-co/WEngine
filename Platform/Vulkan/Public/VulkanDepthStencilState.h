#pragma once

namespace Vulkan
{

	class VulkanDepthStencilState : public RHIDepthStencilState
	{
	public:

		VulkanDepthStencilState(const RHIDepthStencilStateInitializer& Initializer);

		virtual ~VulkanDepthStencilState() = default;

	private:

		VkPipelineDepthStencilStateCreateInfo DepthStencilStateCreateInfo;

		friend class VulkanDevice;
		friend class VulkanPipelineStateManager;

	};

}