#pragma once
#include "RHI/Public/RHISurface.h"

namespace Vulkan
{

	class VulkanSurface
	{
	public:

		VulkanSurface(class VulkanDevice *pInDevice, class VulkanTextureBase *InOwner, VkImageCreateInfo *pInfo, ETextureCreateFlags Flags, EAccess InitState);

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

		VkMemoryRequirements MemoryRequirements;

		VkDeviceMemory DeviceMemory;

		friend VulkanTextureBase;

	};

}