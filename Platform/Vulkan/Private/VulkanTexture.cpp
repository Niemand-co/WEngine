#include "pch.h"
#include "Platform/Vulkan/Public/VulkanTexture.h"
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
	}

	VulkanTexture2D::VulkanTexture2D(VulkanDevice* pInDevice, VkImageCreateInfo* pInfo)
		: VulkanTextureBase(pInDevice, pInfo), RHITexture2D(pInfo->extent.width, pInfo->extent.height, pInfo->mipLevels)
	{
	}

	VulkanTexture2D::~VulkanTexture2D()
	{
	}

	VulkanTexture2DArray::VulkanTexture2DArray(VulkanDevice* pInDevice, VkImageCreateInfo* pInfo)
		: VulkanTextureBase(pInDevice, pInfo), RHITexture2DArray(pInfo->extent.width, pInfo->extent.height, pInfo->mipLevels, pInfo->arrayLayers)
	{
	}

	VulkanTexture2DArray::~VulkanTexture2DArray()
	{
	}

	VulkanTexture3D::VulkanTexture3D(VulkanDevice* pInDevice, VkImageCreateInfo* pInfo)
		: VulkanTextureBase(pInDevice, pInfo), RHITexture3D(pInfo->extent.width, pInfo->extent.height, pInfo->extent.depth, pInfo->mipLevels)
	{
	}

	VulkanTexture3D::~VulkanTexture3D()
	{
	}

}