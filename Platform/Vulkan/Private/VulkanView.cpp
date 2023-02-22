#include "pch.h"
#include "Platform/Vulkan/Public/VulkanView.h"
#include "Platform/Vulkan/Public/VulkanFramebuffer.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Render/Descriptor/Public/RHITextureViewDescriptor.h"

namespace Vulkan
{

	VulkanTextureView::VulkanTextureView(VulkanDevice* pInDevice, VkImageViewCreateInfo* pInfo)
		: pDevice(pInDevice)
	{
		vkCreateImageView(pInDevice->GetHandle(), pInfo, ((VulkanAllocator*)NormalAllocator::Get())->GetCallbacks(), &ImageView);
	}

	VulkanTextureView::~VulkanTextureView()
	{
		vkDestroyImageView(pDevice->GetHandle(), ImageView, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

}