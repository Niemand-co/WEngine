#pragma once
#include "RHI/Public/RHITexture.h"

class RHIDevice;

namespace Vulkan
{

	class VulkanTexture : public RHITexture
	{
	public:

		VulkanTexture(VkImage *image, VkDevice *device);

		virtual ~VulkanTexture();

		virtual RHITextureView* CreateTextureView(RHITextureViewDescriptor *descriptor) override;

	private:

		VkImage *m_image;

		VkDevice *m_device;

	};

}