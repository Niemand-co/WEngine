#include "pch.h"
#include "Platform/Vulkan/Public/VulkanResource.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

namespace Vulkan
{

	void VulkanTextureView::Create(VulkanDevice* Device, VkImage InImage, VkImageViewType ViewType, VkImageAspectFlags AspectFlags, EFormat Format, uint32 FirstMip, uint32 NumMips, uint32 FirstArray, uint32 NumArrays, VkImageUsageFlags UsageFlags)
	{
		VkImageViewCreateInfo ImageViewCreateInfo = {};
		{
			ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			ImageViewCreateInfo.image = InImage;
			ImageViewCreateInfo.viewType = ViewType;
			ImageViewCreateInfo.format = WEngine::ToVulkan(Format);
			ImageViewCreateInfo.components = FormatComponentMapping(Format);
			ImageViewCreateInfo.subresourceRange.aspectMask = AspectFlags;

			ImageViewCreateInfo.subresourceRange.baseMipLevel = FirstMip;
			ImageViewCreateInfo.subresourceRange.levelCount = NumMips;

			ImageViewCreateInfo.subresourceRange.baseArrayLayer = FirstArray;
			switch (ViewType)
			{
			case VK_IMAGE_VIEW_TYPE_1D_ARRAY:
			case VK_IMAGE_VIEW_TYPE_2D_ARRAY:
				ImageViewCreateInfo.subresourceRange.layerCount = NumArrays;
				break;
			case VK_IMAGE_VIEW_TYPE_CUBE:
				ImageViewCreateInfo.subresourceRange.layerCount = 6;
				break;
			case VK_IMAGE_VIEW_TYPE_CUBE_ARRAY:
				ImageViewCreateInfo.subresourceRange.layerCount = 6 * NumArrays;
				break;
			default:
				ImageViewCreateInfo.subresourceRange.layerCount = 1;
				break;
			}

			VkImageViewUsageCreateInfo ImageViewUsageCreateInfo = {};
			if(UsageFlags != 0)
			{
				ImageViewUsageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO;
				ImageViewUsageCreateInfo.pNext = (void*)ImageViewCreateInfo.pNext;
				ImageViewUsageCreateInfo.usage = UsageFlags;
				ImageViewCreateInfo.pNext = &ImageViewUsageCreateInfo;
			}
		}

		RE_ASSERT(vkCreateImageView(Device->GetHandle(), &ImageViewCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &ImageView) == VK_SUCCESS, "Failed to create image view.");

		Image = InImage;
	}

	void VulkanTextureView::Destroy(VulkanDevice* Device)
	{
		if (ImageView)
		{

		}
	}

}