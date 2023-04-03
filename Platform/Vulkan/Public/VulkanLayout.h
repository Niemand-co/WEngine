#pragma once

namespace Vulkan
{
	
	class VulkanLayout : public RHIResource
	{
	public:

		VulkanLayout(class VulkanDevice *pInDevice);

		virtual ~VulkanLayout();

	private:

		VulkanDevice *pDevice;

		VkPipelineLayout PipelineLayout;

	};

}