#include "pch.h"
#include "Platform/Vulkan/Public/VulkanTexture.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanBuffer.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Platform/Vulkan/Public/VulkanContext.h"
#include "Platform/Vulkan/Public/VulkanPipelineBarrier.h"

namespace Vulkan
{

	WEngine::WHashMap<VkImage, VkImageLayout> VulkanTextureLayoutManager::Layouts = WEngine::WHashMap<VkImage, VkImageLayout>();

	VulkanTexture::VulkanTexture(VulkanDevice* pInDevice, const RHITextureDesc& InDesc)
		: RHITexture(InDesc),
		  pDevice(pInDevice)
	{
		VkImageCreateInfo ImageCreateInfo = {};
		{
			ImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			ImageCreateInfo.extent = { InDesc.Extent.width, InDesc.Extent.height, InDesc.Extent.depth };
			ImageCreateInfo.format = WEngine::ToVulkan(InDesc.Format);
			ImageCreateInfo.arrayLayers = 1;
			ImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			ImageCreateInfo.mipLevels = InDesc.NumMips;
			ImageCreateInfo.samples = WEngine::ToVulkan(InDesc.NumSamples);
			ImageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			ImageCreateInfo.usage = GetImageUsage(InDesc.Flags);

			switch (InDesc.Dimension)
			{
			case EDimension::Texture2D:
			case EDimension::Texture2DArray:
				RE_ASSERT(InDesc.Extent.width < pDevice->GetLimits().maxImageDimension2D, "Out of image size limits");
				RE_ASSERT(InDesc.Extent.height < pDevice->GetLimits().maxImageDimension2D, "Out of image size limits");
				ImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
				break;
			case EDimension::TextureCube:
			case EDimension::TextureCubeArray:
				RE_ASSERT(InDesc.Extent.width < pDevice->GetLimits().maxImageDimension2D, "Out of image size limits");
				RE_ASSERT(InDesc.Extent.height < pDevice->GetLimits().maxImageDimension2D, "Out of image size limits");
				ImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
				ImageCreateInfo.arrayLayers = 6;
				ImageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
				break;
			case EDimension::Texture3D:
				RE_ASSERT(InDesc.Extent.width < pDevice->GetLimits().maxImageDimension3D, "Out of image size limits");
				RE_ASSERT(InDesc.Extent.height < pDevice->GetLimits().maxImageDimension3D, "Out of image size limits");
				RE_ASSERT(InDesc.Extent.depth < pDevice->GetLimits().maxImageDimension3D, "Out of image size limits");
				ImageCreateInfo.imageType = VK_IMAGE_TYPE_3D;
				break;
			default:
				RE_ASSERT(false, "Error image dimension.");
			}

			if( InDesc.Dimension == EDimension::Texture1D || InDesc.Dimension == EDimension::Texture1DArray )
			{
				ImageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
				if (InDesc.NumSamples > 1)
				{
					RE_WARN("Not allower to create linear texture with multi sample.");
				}
			}
			else
			{
				ImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			}
		}

		RE_ASSERT(vkCreateImage(pDevice->GetHandle(), &ImageCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &Image), "Failed to create image");

		vkGetImageMemoryRequirements(pDevice->GetHandle(), Image, &MemoryRequirements);

		bool bRenderTarget = WEngine::EnumHasFlags(InDesc.Flags, ETextureCreateFlags::TextureCreate_RenderTarget | ETextureCreateFlags::TextureCreate_DepthStencil | ETextureCreateFlags::TextureCreate_ResolveTarget);
		bool bUAV = WEngine::EnumHasFlags(InDesc.Flags, ETextureCreateFlags::TextureCreate_UAV);
		RE_ASSERT(pDevice->GetMemoryManager()->AllocateImageMemory(Allocation, MemoryRequirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, (bRenderTarget | bUAV) ? EVulkanAllocationMetaType::EAMT_ImageRenderTarget : EVulkanAllocationMetaType::EAMT_ImageOther), "Failed to allocate memory for image.");
		Allocation.BindImage(pDevice, Image);

		VkImageViewType ViewType = GetViewType(InDesc.Dimension);
		VkImageAspectFlags Aspect = GetAspect(InDesc.Format);
		DefaultView.Create(pDevice, Image, ViewType, Aspect, InDesc.Format, 0, WEngine::Max(InDesc.NumMips, (uint8)1), 0, InDesc.ArraySize);

		if (!InDesc.BulkData)
		{
			return;
		}

		VulkanStagingBuffer *StagingBuffer = pDevice->GetStagingBufferManager()->AcquireBuffer(InDesc.BulkDataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
		void *Data = StagingBuffer->GetMappedPointer();
		memcpy(Data, InDesc.BulkData, InDesc.BulkDataSize);

		CopyBufferToImage(StagingBuffer, this, InDesc.Extent.width, InDesc.Extent.height, InDesc.Extent.depth, (ViewType == VK_IMAGE_VIEW_TYPE_CUBE || ViewType == VK_IMAGE_VIEW_TYPE_CUBE_ARRAY) ? InDesc.ArraySize * 6 : InDesc.ArraySize, Aspect);
	}

	VulkanTexture::~VulkanTexture()
	{
		DefaultView.Destroy(pDevice);
		pDevice->GetMemoryManager()->FreeVulkanAllocationImage(Allocation);
	}

	VkImageUsageFlags VulkanTexture::GetImageUsage(ETextureCreateFlags Flag)
	{
		VkImageUsageFlags Usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		if ((Flag & ETextureCreateFlags::TextureCreate_Presentable) != ETextureCreateFlags::TextureCreate_None)
		{
			Usage |= VK_IMAGE_USAGE_STORAGE_BIT;
		}
		else if ((Flag & ETextureCreateFlags::TextureCreate_RenderTarget) != ETextureCreateFlags::TextureCreate_None)
		{
			if ((Flag & ETextureCreateFlags::TextureCreate_InputAttachmentReadable) != ETextureCreateFlags::TextureCreate_None)
			{
				Usage |= (VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
			}
			else
			{
				Usage |= (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
			}
		}
		else if ((Flag & ETextureCreateFlags::TextureCreate_DepthStencil) != ETextureCreateFlags::TextureCreate_None)
		{
			if ((Flag & ETextureCreateFlags::TextureCreate_InputAttachmentReadable) != ETextureCreateFlags::TextureCreate_None)
			{
				Usage |= (VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
			}
			else
			{
				Usage |= (VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
			}
		}

		if ((Flag & ETextureCreateFlags::TextureCreate_UAV) != ETextureCreateFlags::TextureCreate_None)
		{
			Usage |= VK_IMAGE_USAGE_STORAGE_BIT;
		}

		return Usage;
	}

	VkImageViewType VulkanTexture::GetViewType(EDimension Dimension)
	{
		switch (Dimension)
		{
		case EDimension::Texture1D:
			return VK_IMAGE_VIEW_TYPE_1D;
		case EDimension::Texture1DArray:
			return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
		case EDimension::Texture2D:
			return VK_IMAGE_VIEW_TYPE_2D;
		case EDimension::Texture2DArray:
			return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
		case EDimension::TextureCube:
			return VK_IMAGE_VIEW_TYPE_CUBE;
		case EDimension::TextureCubeArray:
			return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
		case EDimension::Texture3D:
			return VK_IMAGE_VIEW_TYPE_3D;
		default:
			RE_ASSERT(false, "Not matched view type.");
			return VkImageViewType();
		}
	}

	VkImageAspectFlags VulkanTexture::GetAspect(EFormat Format)
	{
		switch (Format)
		{
		case EFormat::D32_SFloat:
		case EFormat::D16_Unorm:
			return VK_IMAGE_ASPECT_DEPTH_BIT;
		case EFormat::S8_UINT:
			return VK_IMAGE_ASPECT_STENCIL_BIT;
		case EFormat::D16_UNORM_S8_UINT:
		case EFormat::D24_UNORM_S8_UINT:
		case EFormat::D32_SFLOAT_S8_UINT:
			return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		default:
			return VK_IMAGE_ASPECT_COLOR_BIT;
		}
	}

	void VulkanTexture::CopyBufferToImage(VulkanStagingBuffer *StagingBuffer, VulkanTexture *Texture, uint32 DstSizeX, uint32 DstSizeY, uint32 DstSizeZ, uint32 DstLayerCount, VkImageAspectFlags Aspect)
	{
		VkBuffer SrcBuffer = StagingBuffer->GetHandle();
		VkImage DstImage = Texture->GetHandle();

		VkBufferImageCopy Region = {};
		{
			Region.bufferOffset = 0;
			Region.bufferRowLength = DstSizeX;
			Region.bufferImageHeight = DstSizeY;

			Region.imageSubresource.mipLevel = 0;
			Region.imageSubresource.baseArrayLayer = 0;
			Region.imageSubresource.aspectMask = Aspect;
			Region.imageSubresource.layerCount = DstLayerCount;

			Region.imageExtent.width = DstSizeX;
			Region.imageExtent.height = DstSizeY;
			Region.imageExtent.depth = DstSizeZ;
		}

		VulkanCommandBuffer *CmdBuffer = static_cast<VulkanDynamicContext*>(GetDynamicRHI())->GetCmdBufferManager()->GetImmediateCommandBuffer();

		{
			VulkanPipelineBarrier Barrier;
			Barrier.AddTransition(DstImage, Aspect, 0, 0, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
			Barrier.Execute(CmdBuffer);
		}
		
		vkCmdCopyBufferToImage(CmdBuffer->GetHandle(), SrcBuffer, DstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &Region);

		{
			VulkanPipelineBarrier Barrier;
			Barrier.AddTransition(DstImage, Aspect, 0, 0, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_UNDEFINED);
			Barrier.Execute(CmdBuffer);
		}

		Texture->pDevice->GetStagingBufferManager()->ReleaseBuffer(StagingBuffer);
	}

}