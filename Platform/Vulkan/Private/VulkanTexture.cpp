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

	WEngine::WHashMap<VkImage, VkImageLayout> VulkanTextureLayoutManager::Layouts = WEngine::WHashMap<VkImage, VkImageLayout>();

	VulkanTextureBase::VulkanTextureBase(VulkanDevice* pInDevice, VkImageCreateInfo* pInfo, VkImageViewType ViewType, ETextureCreateFlags Flags, EAccess InitState)
		: pDevice(pInDevice),
		  Surface(pInDevice, this, pInfo, ViewType, Flags, InitState)
	{

	}

	VulkanTextureBase::~VulkanTextureBase()
	{

	}

	VulkanTexture2D::VulkanTexture2D(VulkanDevice* pInDevice, VkImageCreateInfo* pInfo, Format InPixelFormat, ETextureCreateFlags Flags, EAccess InitState, ClearValue InClearValue, uint32 InSampleCount)
		: VulkanTextureBase(pInDevice, pInfo, VK_IMAGE_VIEW_TYPE_2D, Flags, InitState), RHITexture2D(InPixelFormat, pInfo->extent.width, pInfo->extent.height, pInfo->mipLevels, InClearValue, InSampleCount, Flags)
	{
	}

	VulkanTexture2D::~VulkanTexture2D()
	{
	}

	VulkanTexture2DArray::VulkanTexture2DArray(VulkanDevice* pInDevice, VkImageCreateInfo* pInfo, Format InPixelFormat, ETextureCreateFlags Flags, EAccess InitState, ClearValue InClearValue, uint32 InSampleCount)
		: VulkanTextureBase(pInDevice, pInfo, VK_IMAGE_VIEW_TYPE_2D_ARRAY, Flags, InitState), RHITexture2DArray(InPixelFormat, pInfo->extent.width, pInfo->extent.height, pInfo->mipLevels, pInfo->arrayLayers, InClearValue, InSampleCount, Flags)
	{
	}

	VulkanTexture2DArray::~VulkanTexture2DArray()
	{
	}

	VulkanTexture3D::VulkanTexture3D(VulkanDevice* pInDevice, VkImageCreateInfo* pInfo, Format InPixelFormat, ETextureCreateFlags Flags, EAccess InitState, ClearValue InClearValue, uint32 InSampleCount)
		: VulkanTextureBase(pInDevice, pInfo, VK_IMAGE_VIEW_TYPE_3D, Flags, InitState), RHITexture3D(InPixelFormat, pInfo->extent.width, pInfo->extent.height, pInfo->extent.depth, pInfo->mipLevels, InClearValue, InSampleCount, Flags)
	{
	}

	VulkanTexture3D::~VulkanTexture3D()
	{
	}

}