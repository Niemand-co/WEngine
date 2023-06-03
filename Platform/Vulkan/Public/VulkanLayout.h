#pragma once

namespace Vulkan
{
	
	class VulkanLayout : public VulkanResource
	{
	public:

		VulkanLayout(class VulkanDevice *pInDevice, const class VulkanDescriptorSetLayout& DescriptorSetLayout);

		virtual ~VulkanLayout();

		VkPipelineLayout GetHandle() const { return PipelineLayout; }

	private:

		VulkanDevice *pDevice;

		VkPipelineLayout PipelineLayout;

	};

}