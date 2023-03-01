#include "pch.h"
#include "Platform/Vulkan/Public/VulkanSurface.h"
#include "Platform/Vulkan/Public/VulkanGPU.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanContext.h"
#include "platform/Vulkan/Public/VulkanTexture.h"
#include "Platform/Vulkan/Public/VulkanPipelineBarrier.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"

namespace Vulkan
{

	static VkImageLayout GetInitialLayoutForAccess(EAccess Access, ETextureCreateFlags Flags)
	{
		if (WEngine::EnumHasFlags(Access, EAccess::RTV) || Access == EAccess::Present)
		{
			return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}

		if (WEngine::EnumHasFlags(Access, EAccess::SRV))
		{
			return WEngine::EnumHasFlags(Flags, ETextureCreateFlags::TextureCreate_DepthStencil) ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}

		if (WEngine::EnumHasFlags(Access, EAccess::UAV))
		{
			return VK_IMAGE_LAYOUT_GENERAL;
		}

		if (Access == EAccess::Unknown)
		{
			return VK_IMAGE_LAYOUT_UNDEFINED;
		}

		if (Access == EAccess::CopySrc)
		{
			return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		}

		if (Access == EAccess::CopyDst)
		{
			return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		}

		return VK_IMAGE_LAYOUT_UNDEFINED;
	}

	bool IsDepthFormat(VkFormat Format)
	{
		return Format == VK_FORMAT_D16_UNORM || Format == VK_FORMAT_D16_UNORM_S8_UINT || Format == VK_FORMAT_D24_UNORM_S8_UINT || Format == VK_FORMAT_D32_SFLOAT || Format == VK_FORMAT_D32_SFLOAT_S8_UINT;
	}

	bool IsStencilFormat(VkFormat Format)
	{
		return Format == VK_FORMAT_S8_UINT || Format == VK_FORMAT_D16_UNORM_S8_UINT || Format == VK_FORMAT_D24_UNORM_S8_UINT || Format == VK_FORMAT_D32_SFLOAT_S8_UINT;
	}

	VulkanSurface::VulkanSurface(VulkanDevice *pInDevice, VulkanTextureBase* InOwner, VkImageCreateInfo* pInfo, ETextureCreateFlags Flags, EAccess InitState)
		: pDevice(pInDevice),
		  Owner(InOwner),
		  Width(pInfo->extent.width),
		  Height(pInfo->extent.height),
		  Depth(pInfo->extent.depth),
		  NumArray(pInfo->arrayLayers),
		  NumSample(pInfo->samples),
		  NumMip(pInfo->mipLevels),
		  format(pInfo->format),
		  Aspect(0)
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

		Aspect = (IsDepthFormat(format)) ? ( IsStencilFormat(format) ? (VK_IMAGE_ASPECT_STENCIL_BIT | VK_IMAGE_ASPECT_DEPTH_BIT) : VK_IMAGE_ASPECT_DEPTH_BIT ) : ( IsStencilFormat(format) ? VK_IMAGE_ASPECT_STENCIL_BIT : VK_IMAGE_ASPECT_COLOR_BIT );


		VkImageLayout InitLayout = GetInitialLayoutForAccess(InitState, Flags);

		VulkanPipelineBarrier Barrier;
		VkImageSubresourceRange Range = VulkanPipelineBarrier::GetTextureSubresourceRange(Aspect);
		Barrier.AddTransition(Image, Range, VK_IMAGE_LAYOUT_UNDEFINED, InitLayout);
		VulkanCommandBuffer *CmdBuffer = static_cast<VulkanContext*>(RHIContext::GetContext())->GetCmdBufferManager()->GetImmediateCommandBuffer();
		Barrier.Execute(CmdBuffer);

		VulkanTextureLayoutManager::AddLayout(Image, InitLayout);
	}

	VulkanSurface::~VulkanSurface()
	{
		vkDestroyImage(pDevice->GetHandle(), Image, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
		vkFreeMemory(pDevice->GetHandle(), DeviceMemory, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

}
