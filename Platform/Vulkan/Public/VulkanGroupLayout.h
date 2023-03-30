#pragma once
#include "RHI/Public/RHIGroupLayout.h"

namespace Vulkan
{

	class VulkanDescriptorSetLayout : public RHIDescriptorSetLayout
	{
	public:

		VulkanDescriptorSetLayout(class VulkanDevice* pInDevice);

		virtual ~VulkanDescriptorSetLayout();

		VkDescriptorSetLayout GetHandle() const { return DescriptorSetLayout; }

	private:

		VulkanDevice *pDevice;

		VkDescriptorSetLayout DescriptorSetLayout;

		WEngine::WArray<VkDescriptorSetLayoutBinding> Bindings;

	};

	class VulkanDescriptorSetLayoutManager : public RHIResource
	{

	};

}