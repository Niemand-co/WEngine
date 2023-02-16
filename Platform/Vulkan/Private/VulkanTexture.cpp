#include "pch.h"
#include "Platform/Vulkan/Public/VulkanTexture.h"
#include "Platform/Vulkan/Public/VulkanTextureView.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanGPU.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIContext.h"
#include "Render/Descriptor/Public/RHITextureViewDescriptor.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"
#include "Utils/Public/FileLoader.h"

namespace Vulkan
{

	VulkanTextureBase::VulkanTextureBase(VulkanDevice* pInDevice, VkImageCreateInfo* pInfo)
		: pDevice(pInDevice)
	{
		vkCreateImage(pInDevice->GetHandle(), pInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &Image);

		vkGetImageMemoryRequirements(pInDevice->GetHandle(), Image, &MemoryRequirements);

		unsigned int index = 0;
		GPUFeature feature = pInDevice->GetGPU()->GetFeature();
		for (; index < feature.memorySupports.Size(); ++index)
		{
			if ((MemoryRequirements.memoryTypeBits & 1) && (feature.memorySupports[index]->properties & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
				break;
			MemoryRequirements.memoryTypeBits >>= 1;
		}

		VkMemoryAllocateInfo MemoryAllocateInfo = {};
		{
			MemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			MemoryAllocateInfo.allocationSize = MemoryRequirements.size;
			MemoryAllocateInfo.memoryTypeIndex = index;
		}
		vkAllocateMemory(pInDevice->GetHandle(), &MemoryAllocateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &DeviceMemory);

		vkBindImageMemory(pInDevice->GetHandle(), Image, DeviceMemory, 0);
	}

	VulkanTextureBase::~VulkanTextureBase()
	{
		vkDestroyImage(pDevice->GetHandle(), Image, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
		vkFreeMemory(pDevice->GetHandle(), DeviceMemory, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
		vkDestroyImageView(pDevice->GetHandle(), ImageView, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	void VulkanTextureBase::CreateTextureView(VkImageViewCreateInfo* pInfo)
	{
		vkCreateImageView(pDevice->GetHandle(), pInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &ImageView);
	}

	VulkanTexture2D::VulkanTexture2D(VulkanDevice* pInDevice, VkImageCreateInfo* pInfo, uint32 Aspect)
		: VulkanTextureBase(pInDevice, pInfo), RHITexture2D(pInfo->extent.width, pInfo->extent.height)
	{
		VkImageViewCreateInfo ImageViewCreateInfo = {};
		{
			ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			ImageViewCreateInfo.image = Image;
			ImageViewCreateInfo.format = pInfo->format;
			ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			ImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
			ImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
			ImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
			ImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
			ImageViewCreateInfo.subresourceRange.levelCount = pInfo->mipLevels;
			ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			ImageViewCreateInfo.subresourceRange.layerCount = pInfo->arrayLayers;
			ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			ImageViewCreateInfo.subresourceRange.aspectMask = Aspect;
		}
		CreateTextureView(&ImageViewCreateInfo);
	}

	VulkanTexture2D::~VulkanTexture2D()
	{
	}

	VulkanTexture2DArray::VulkanTexture2DArray(VulkanDevice* pInDevice, VkImageCreateInfo* pInfo, uint32 Aspect)
		: VulkanTextureBase(pInDevice, pInfo), RHITexture2DArray(pInfo->extent.width, pInfo->extent.height, pInfo->arrayLayers)
	{
		VkImageViewCreateInfo ImageViewCreateInfo = {};
		{
			ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			ImageViewCreateInfo.image = Image;
			ImageViewCreateInfo.format = pInfo->format;
			ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
			ImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
			ImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
			ImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
			ImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
			ImageViewCreateInfo.subresourceRange.levelCount = pInfo->mipLevels;
			ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			ImageViewCreateInfo.subresourceRange.layerCount = pInfo->arrayLayers;
			ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			ImageViewCreateInfo.subresourceRange.aspectMask = Aspect;
		}
		CreateTextureView(&ImageViewCreateInfo);
	}

	VulkanTexture2DArray::~VulkanTexture2DArray()
	{
	}

	VulkanTexture3D::VulkanTexture3D(VulkanDevice* pInDevice, VkImageCreateInfo* pInfo, uint32 Aspect)
		: VulkanTextureBase(pInDevice, pInfo), RHITexture3D(pInfo->extent.width, pInfo->extent.height, pInfo->extent.depth)
	{
		VkImageViewCreateInfo ImageViewCreateInfo = {};
		{
			ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			ImageViewCreateInfo.image = Image;
			ImageViewCreateInfo.format = pInfo->format;
			ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_3D;
			ImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
			ImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
			ImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
			ImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
			ImageViewCreateInfo.subresourceRange.levelCount = pInfo->mipLevels;
			ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			ImageViewCreateInfo.subresourceRange.layerCount = pInfo->arrayLayers;
			ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			ImageViewCreateInfo.subresourceRange.aspectMask = Aspect;
		}
		CreateTextureView(&ImageViewCreateInfo);
	}

	VulkanTexture3D::~VulkanTexture3D()
	{
	}

}