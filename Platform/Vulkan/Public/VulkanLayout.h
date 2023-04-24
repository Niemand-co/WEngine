#pragma once

namespace Vulkan
{
	
	class VulkanLayout : public VulkanResource
	{
	public:

		VulkanLayout(class VulkanDevice *pInDevice);

		virtual ~VulkanLayout();

	private:

		VulkanDevice *pDevice;

		VkPipelineLayout PipelineLayout;

	};

}