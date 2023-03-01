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

	VulkanTextureBase::VulkanTextureBase(VulkanDevice* pInDevice, VkImageCreateInfo* pInfo, ETextureCreateFlags Flags, EAccess InitState)
		: pDevice(pInDevice),
		  Surface(pInDevice, this, pInfo, Flags, InitState)
	{

	}

	VulkanTextureBase::~VulkanTextureBase()
	{

	}

	VulkanTexture2D::VulkanTexture2D(VulkanDevice* pInDevice, VkImageCreateInfo* pInfo, ETextureCreateFlags Flags, EAccess InitState)
		: VulkanTextureBase(pInDevice, pInfo, Flags, InitState), RHITexture2D(pInfo->extent.width, pInfo->extent.height, pInfo->mipLevels)
	{
	}

	VulkanTexture2D::~VulkanTexture2D()
	{
	}

	VulkanTexture2DArray::VulkanTexture2DArray(VulkanDevice* pInDevice, VkImageCreateInfo* pInfo, ETextureCreateFlags Flags, EAccess InitState)
		: VulkanTextureBase(pInDevice, pInfo, Flags, InitState), RHITexture2DArray(pInfo->extent.width, pInfo->extent.height, pInfo->mipLevels, pInfo->arrayLayers)
	{
	}

	VulkanTexture2DArray::~VulkanTexture2DArray()
	{
	}

	VulkanTexture3D::VulkanTexture3D(VulkanDevice* pInDevice, VkImageCreateInfo* pInfo, ETextureCreateFlags Flags, EAccess InitState)
		: VulkanTextureBase(pInDevice, pInfo, Flags, InitState), RHITexture3D(pInfo->extent.width, pInfo->extent.height, pInfo->extent.depth, pInfo->mipLevels)
	{
	}

	VulkanTexture3D::~VulkanTexture3D()
	{
	}

}