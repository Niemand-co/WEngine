#pragma once
#include "RHI/Public/RHIVertexInputState.h"

namespace Vulkan
{

	class VulkanVertexInputState : public RHIVertexInputState
	{
	public:

		VulkanVertexInputState(const WEngine::WArray<VertexInputElement>& InElements);

		virtual ~VulkanVertexInputState() = default;

	private:

		VkVertexInputBindingDescription VertexInputBindings[MaxVertexInputElementCount] = {};

		VkVertexInputAttributeDescription VertexInputAttributes[MaxVertexInputElementCount] = {};

		VkPipelineVertexInputStateCreateInfo VertexInputStateCreateInfo = {};

		uint32 NumVertexBindings = 0;

		uint32 NumVertexAttributes = 0;

	};

}