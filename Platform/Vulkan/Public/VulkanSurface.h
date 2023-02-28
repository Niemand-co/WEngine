#pragma once
#include "RHI/Public/RHISurface.h"

namespace Vulkan
{

	class VulkanSurface
	{
	public:

		VulkanSurface(class VulkanDevice *pInDevice, class VulkanTextureBase *InOwner, uint32 InWidth, uint32 InHeight, uint32 InDepth, uint32 InNumArray, uint32 InNumSample, uint32 InNumMip, VkFormat InFormat);

		virtual ~VulkanSurface();

		VkImage GetImage() const { return Image; }

		VkFormat GetFormat() const { return format; }

		VkImageAspectFlags GetImageAspect() const { return Aspect; }

	private:

		VulkanDevice* pDevice;

		VulkanTextureBase *Owner;

		uint32 Width;

		uint32 Height;

		uint32 Depth;

		uint32 NumArray;

		uint32 NumSample;

		uint32 NumMip;

		VkImage Image;

		VkFormat format;

		VkImageAspectFlags Aspect;

		friend VulkanTextureBase;

	};

}