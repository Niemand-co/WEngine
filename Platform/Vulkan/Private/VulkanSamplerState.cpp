#include "pch.h"
#include "Platform/Vulkan/Public/VulkanSamplerState.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

namespace Vulkan
{

	VulkanSamplerState::VulkanSamplerState(VulkanDevice* pInDevice, const VkSamplerCreateInfo& Info)
	{
		vkCreateSampler(pInDevice->GetHandle(), &Info, ResourceCast(NormalAllocator::Get())->GetCallbacks(), &Sampler);
	}

	void VulkanSamplerState::SetupCreateInfo(const RHISamplerStateInitializer& Initializer, VkSamplerCreateInfo& OutInfo)
	{
		ZeroVulkanStruct(OutInfo, VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO);

		OutInfo.addressModeU = WEngine::ToVulkan(Initializer.ModeU);
		OutInfo.addressModeV = WEngine::ToVulkan(Initializer.ModeV);
		OutInfo.addressModeW = WEngine::ToVulkan(Initializer.ModeW);
		OutInfo.minFilter = WEngine::ToVulkan(Initializer.Filter);
		OutInfo.magFilter = WEngine::ToVulkan(Initializer.Filter);

		OutInfo.mipLodBias = Initializer.MipBias;
	}

}