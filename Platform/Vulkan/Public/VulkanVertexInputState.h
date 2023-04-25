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

		WEngine::WArray<VkVertexInputBindingDescription> Bindings;

		uint32 NumAttributes = 0;

		WEngine::WHashMap<uint32, uint32> BindingToStream;

		friend class VulkanDevice;
		friend class VulkanPipelineStateManager;

	};

	class VulkanVertexInputStateManager
	{
	public:

		static VulkanVertexInputState* GetVertexInput(uint32 ID)
		{
			if (VertexInputs.Find(ID))
			{
				return VertexInputs[ID];
			}
			return nullptr;
		}

		static void AddVertexInput(uint32 ID, VulkanVertexInputState* VertexInput)
		{
			VertexInputs[ID] = VertexInput;
		}

	public:

		static WEngine::WHashMap<uint32, VulkanVertexInputState*> VertexInputs;

	};

}