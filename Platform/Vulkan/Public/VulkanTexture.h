#pragma once
#include "RHI/Public/RHITexture.h"

class RHIDevice;

namespace Vulkan
{

	class VulkanTexture : public RHITexture
	{
	public:

		VulkanTexture(VkImage *image, VkDevice *device, bool isDisplay = false);

		virtual ~VulkanTexture();

		virtual RHITextureView* CreateTextureView(RHITextureViewDescriptor *descriptor) override;

		VkImage* GetHandle();

		void SetHandle(VkImage *image);

	private:

		VkImage *m_pImage;

		VkDevice *m_pDevice;

		bool m_isDisplayTexture;

	};

}