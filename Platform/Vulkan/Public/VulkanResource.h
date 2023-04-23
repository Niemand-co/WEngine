#pragma once

namespace Vulkan
{

	class VulkanResource
	{
	public:

		void* operator new(size_t size)
		{
			return NormalAllocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			NormalAllocator::Get()->Deallocate(pData);
		}

	};

	struct VulkanTextureView
	{
		VulkanTextureView()
			: ImageView(VK_NULL_HANDLE),
			  Image(VK_NULL_HANDLE)
		{
		}

		void Create(class VulkanDevice *Device, VkImage InImage, VkImageViewType ViewType, VkImageAspectFlags AspectFlags, EFormat Format, uint32 FirstMip, uint32 NumMips, uint32 FirstArray, uint32 NumArrays, VkImageUsageFlags UsageFlags = 0);

		void Destroy(class VulkanDevice *Device);

		VkImageView ImageView;
		VkImage Image;
	};

}