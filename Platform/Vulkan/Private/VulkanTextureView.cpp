#include "pch.h"
#include "Platform/Vulkan/Public/VulkanTextureView.h"
#include "Platform/Vulkan/Public/VulkanFramebuffer.h"
#include "Render/Descriptor/Public/RHITextureViewDescriptor.h"

namespace Vulkan
{

	VulkanTextureView::VulkanTextureView(VkImageView* imageView, RHITextureViewDescriptor* descriptor)
	{
		m_format = descriptor->format;
		m_mipCount = descriptor->mipCount;
		m_baseMipLevel = descriptor->baseMipLevel;
		m_arrayLayerCount = descriptor->arrayLayerCount;
		m_baseArrayLayer = descriptor->baseArrayLayer;
		m_dimension = descriptor->dimension;
		m_imageView = imageView;
	}

	VulkanTextureView::~VulkanTextureView()
	{

	}

	VkImageView* VulkanTextureView::GetHandle()
	{
		return m_imageView;
	}

}