#include "pch.h"
#include "Platform/Vulkan/Public/VulkanSurface.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

namespace Vulkan
{

	bool IsDepthFormat(VkFormat Format)
	{
		return Format == VK_FORMAT_D16_UNORM || Format == VK_FORMAT_D16_UNORM_S8_UINT || Format == VK_FORMAT_D24_UNORM_S8_UINT || Format == VK_FORMAT_D32_SFLOAT || Format == VK_FORMAT_D32_SFLOAT_S8_UINT;
	}

	bool IsStencilFormat(VkFormat Format)
	{
		return Format == VK_FORMAT_S8_UINT || Format == VK_FORMAT_D16_UNORM_S8_UINT || Format == VK_FORMAT_D24_UNORM_S8_UINT || Format == VK_FORMAT_D32_SFLOAT_S8_UINT;
	}

	VulkanSurface::VulkanSurface(VulkanDevice *pInDevice, VulkanTextureBase* InOwner, uint32 InWidth, uint32 InHeight, uint32 InDepth, uint32 InNumArray, uint32 InNumSample, uint32 InNumMip, VkFormat InFormat)
		: pDevice(pInDevice),
		  Owner(InOwner),
		  Width(InWidth),
		  Height(InHeight),
		  Depth(InDepth),
		  NumArray(InNumArray),
		  NumSample(InNumSample),
		  NumMip(InNumMip),
		  format(InFormat),
		  Aspect(0)
	{
		Aspect = (IsDepthFormat(format)) ? ( IsStencilFormat(format) ? (VK_IMAGE_ASPECT_STENCIL_BIT | VK_IMAGE_ASPECT_DEPTH_BIT) : VK_IMAGE_ASPECT_DEPTH_BIT ) : ( IsStencilFormat(format) ? VK_IMAGE_ASPECT_STENCIL_BIT : VK_IMAGE_ASPECT_COLOR_BIT );
	}

	VulkanSurface::~VulkanSurface()
	{
		vkDestroyImage(pDevice->GetHandle(), Image, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

}
