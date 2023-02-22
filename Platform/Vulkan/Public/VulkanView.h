#pragma once
#include "RHI/Public/RHIView.h"

namespace Vulkan
{

	class VulkanTextureViewBase
	{
	public:

		VulkanTextureViewBase(class VulkanDevice *pInDevice, VkImageViewCreateInfo *pInfo);

		virtual ~VulkanTextureViewBase();

		VkImageView GetHandle() const { return ImageView; }

	private:

		friend class VulkanFramebuffer;

		VulkanDevice *pDevice;

		VkImageView ImageView;

	};

	class VulkanTextureSRV : public RHITextureSRV, public VulkanTextureViewBase
	{
	public:

		VulkanTextureSRV(VulkanDevice *pInDevice, VkImageViewCreateInfo *pInfo);

		virtual ~VulkanTextureSRV();

	};

	class VulkanTextureUAV : public RHITextureUAV, public VulkanTextureViewBase
	{
	public:

		VulkanTextureUAV(VulkanDevice *pInDevice, VkImageViewCreateInfo *pInfo);

		virtual ~VulkanTextureUAV();

	};

	class VulkanTextureRTV : public RHITextureRTV, public VulkanTextureViewBase
	{
	public:

		VulkanTextureRTV(VulkanDevice *pInDevice, VkImageViewCreateInfo *pInfo);

		virtual ~VulkanTextureRTV();

	};

}