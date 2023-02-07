#include "pch.h"
#include "Platform/Vulkan/Public/VulkanTexture.h"
#include "Platform/Vulkan/Public/VulkanTextureView.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIContext.h"
#include "Render/Descriptor/Public/RHITextureViewDescriptor.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"
#include "Utils/Public/FileLoader.h"

namespace Vulkan
{

	VulkanTextureBase::VulkanTextureBase(VkImage* pInImage)
		: pImage(pInImage)
	{
	}

	VulkanTextureBase::~VulkanTextureBase()
	{
	}

	VulkanTexture2D::VulkanTexture2D(VkImage* pImage, uint32 width, uint32 height)
		: VulkanTextureBase(pImage), RHITexture2D(width, height)
	{
	}

	VulkanTexture2D::~VulkanTexture2D()
	{
	}

	RHITextureView* VulkanTexture2D::CreateTextureView(RHITextureViewDescriptor* descriptor)
	{
		return nullptr;
	}

}