#include "pch.h"
#include "Platform/Vulkan/Public/VulkanSamplerState.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

namespace Vulkan
{

	VulkanSamplerState::VulkanSamplerState(VulkanDevice* pInDevice, const VkSamplerCreateInfo& Info)
	{
		vkCreateSampler(pInDevice->GetHandle(), &Info, ResourceCast(NormalAllocator::Get())->GetCallbacks(), &Sampler);
	}

	void VulkanSamplerState::SetupCreateInfo(const RHISamplerStateInitializer& Initializer, VulkanDevice *pDevice, VkSamplerCreateInfo& OutInfo)
	{
		ZeroVulkanStruct(OutInfo, VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO);

		OutInfo.addressModeU = WEngine::ToVulkan(Initializer.ModeU);
		OutInfo.addressModeV = WEngine::ToVulkan(Initializer.ModeV);
		OutInfo.addressModeW = WEngine::ToVulkan(Initializer.ModeW);
		OutInfo.minFilter = WEngine::ToVulkan(Initializer.Filter);
		OutInfo.magFilter = WEngine::ToVulkan(Initializer.Filter);

		OutInfo.mipLodBias = Initializer.MipBias;

		OutInfo.maxAnisotropy = 1.0f;
		if (Initializer.Filter == EFilter::FL_AnisotropicLinear || Initializer.Filter == EFilter::FL_AnisotropicNearest)
		{
			OutInfo.maxAnisotropy = WEngine::Clamp((float)Initializer.MaxAnisotropy, 1.0f, pDevice->GetLimits().maxSamplerAnisotropy);
		}
		OutInfo.anisotropyEnable = OutInfo.maxAnisotropy > 1.0f;

		OutInfo.compareEnable = Initializer.Comparision != ESamplerCompareFunction::SCF_Never ? VK_TRUE : VK_FALSE;
		OutInfo.compareOp = WEngine::ToVulkan(Initializer.Comparision);
		OutInfo.minLod = 0;
		OutInfo.maxLod = FLT_MAX;
		OutInfo.borderColor = Initializer.BorderColor == 0 ? VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK : VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	}

}