#pragma once
#include "RHI/Public/RHIGroupLayout.h"

namespace Vulkan
{

	class VulkanGroupLayout : public RHIGroupLayout
	{
	public:

		VulkanGroupLayout(class VulkanDevice* pInDevice);

		virtual ~VulkanGroupLayout();

		VkDescriptorSetLayout GetHandle() const { return DescriptorSetLayout; }

	private:

		VulkanDevice *pDevice;

		VkDescriptorSetLayout DescriptorSetLayout;

		WEngine::WArray<VkDescriptorSetLayoutBinding> Bindings;

	};

}