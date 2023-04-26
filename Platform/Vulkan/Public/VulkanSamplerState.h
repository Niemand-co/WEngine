#pragma once
#include "RHI/Public/RHISamplerState.h"

namespace Vulkan
{

	class VulkanSamplerState : public RHISamplerState
	{
	public:

		VulkanSamplerState(class VulkanDevice *pInDevice, const VkSamplerCreateInfo& Info);

		virtual ~VulkanSamplerState() = default;

		static void SetupCreateInfo(const RHISamplerStateInitializer& Initializer, VkSamplerCreateInfo& OutInfo);

	private:

		VkSampler Sampler;

	};

	class VulkanSamplerStateManager : public VulkanResource
	{
	public:

		VulkanSamplerStateManager(VulkanDevice* pInDevice)
			: pDevice(pInDevice)
		{
		}

	private:

		VulkanDevice *pDevice;

		WEngine::WHashMap<uint32, VulkanSamplerState*> SamplerStates;

		friend class VulkanDevice;

	};

}