#pragma once
#include "RHI/Public/RHIView.h"

namespace Vulkan
{

	class VulkanTextureView : public RHITextureView
	{
	public:

		VulkanTextureView(class VulkanDevice *pInDevice, VkImageViewCreateInfo *pInfo);

		virtual ~VulkanTextureView();

		VkImageView GetHandle() const { return ImageView; }

	private:

		friend class VulkanFramebuffer;

		VulkanDevice *pDevice;

		VkImageView ImageView;

	};

}