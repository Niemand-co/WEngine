#include "pch.h"
#include "Platform/Vulkan/Public/VulkanView.h"
#include "Platform/Vulkan/Public/VulkanFramebuffer.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Render/Descriptor/Public/RHITextureViewDescriptor.h"

namespace Vulkan
{

	VulkanTextureViewBase::VulkanTextureViewBase(VulkanDevice* pInDevice, VkImageViewCreateInfo* pInfo)
		: pDevice(pInDevice)
	{
		vkCreateImageView(pInDevice->GetHandle(), pInfo, ((VulkanAllocator*)NormalAllocator::Get())->GetCallbacks(), &ImageView);
	}

	VulkanTextureViewBase::~VulkanTextureViewBase()
	{
		vkDestroyImageView(pDevice->GetHandle(), ImageView, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	VulkanTextureSRV::VulkanTextureSRV(VulkanDevice* pInDevice, VkImageViewCreateInfo* pInfo)
		: VulkanTextureViewBase(pInDevice, pInfo)
	{
	}

	VulkanTextureSRV::~VulkanTextureSRV()
	{
	}

	VulkanTextureUAV::VulkanTextureUAV(VulkanDevice* pInDevice, VkImageViewCreateInfo* pInfo)
		: VulkanTextureViewBase(pInDevice, pInfo)
	{
	}

	VulkanTextureUAV::~VulkanTextureUAV()
	{
	}

	VulkanTextureRTV::VulkanTextureRTV(VulkanDevice* pInDevice, VkImageViewCreateInfo* pInfo)
		: VulkanTextureViewBase(pInDevice, pInfo)
	{
	}

	VulkanTextureRTV::~VulkanTextureRTV()
	{
	}

}