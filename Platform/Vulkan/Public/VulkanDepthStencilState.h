#pragma once
#include "RHI/Public/RHIDepthStencilState.h"

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

	};

}