#include "pch.h"
#include "Platform/Vulkan/Public/VulkanTexture.h"
#include "Platform/Vulkan/Public/VulkanTextureView.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "RHI/Public/RHIDevice.h"
#include "Render/Descriptor/Public/RHITextureViewDescriptor.h"

namespace Vulkan
{
	VulkanTexture::VulkanTexture(VkImage* image, VkDevice *device, bool isDisplay)
		: m_pImage(image), m_pDevice(device), m_isDisplayTexture(isDisplay)
	{
	}

	VulkanTexture::~VulkanTexture()
	{
		if(!m_isDisplayTexture)
			vkDestroyImage(*m_pDevice, *m_pImage, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks());
	}

	RHITextureView* VulkanTexture::CreateTextureView(RHITextureViewDescriptor* descriptor)
	{
		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = *m_pImage;
		imageViewCreateInfo.viewType = WEngine::ToVulkan(descriptor->dimension);
		imageViewCreateInfo.format = WEngine::ToVulkan(descriptor->format);
		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = descriptor->baseMipLevel;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = descriptor->baseArrayLayer;
		imageViewCreateInfo.subresourceRange.levelCount = descriptor->mipCount;
		imageViewCreateInfo.subresourceRange.layerCount = descriptor->arrayLayerCount;

		VkImageView* imageView = (VkImageView*)WEngine::Allocator::Get()->Allocate(sizeof(VkImageView));
		RE_ASSERT(vkCreateImageView(*m_pDevice, &imageViewCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), imageView) == VK_SUCCESS, "Failed to Create Image View.");

		RHITextureView *textureView = (RHITextureView*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanTextureView));
		::new (textureView) VulkanTextureView(imageView, m_pDevice, descriptor);

		return textureView;
	}

	VkImage* VulkanTexture::GetHandle()
	{
		return m_pImage;
	}

	void VulkanTexture::SetHandle(VkImage* image)
	{
		m_pImage = image;
	}

}