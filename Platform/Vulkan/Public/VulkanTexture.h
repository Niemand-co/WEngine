#pragma once
#include "RHI/Public/RHITexture.h"
#include "Platform/Vulkan/Allocator/Public/VulkanMemoryManager.h"

namespace Vulkan
{

	class VulkanTextureLayoutManager
	{
	public:

		static VkImageLayout FindLayout(VkImage Image)
		{
			RE_ASSERT(Layouts.Find(Image), "No Such Image Recorded.");
			return Layouts[Image];
		}

		static void AddLayout(VkImage Image, VkImageLayout Layout)
		{
			Layouts.Insert(Image, Layout);
		}

	private:

		static WEngine::WHashMap<VkImage, VkImageLayout> Layouts;

	};

	class VulkanTexture : public RHITexture
	{
	public:

		VulkanTexture(class VulkanDevice* pInDevice, const RHITextureDesc& InDesc);

		virtual ~VulkanTexture();

		VkImage GetHandle() const { return Image; }

		VkImageView GetViewHandle() const { return DefaultView.ImageView; }

		VkImageUsageFlags GetImageUsage(ETextureCreateFlags Flag);

		VkImageViewType GetViewType(EDimension Dimension);

		VkImageAspectFlags GetAspect(EFormat Format);

		static void CopyBufferToImage(class VulkanStagingBuffer* StagingBuffer, VulkanTexture* Texture, uint32 DstSizeX, uint32 DstSizeY, uint32 DstSizeZ, uint32 DstLayerCount, VkImageAspectFlags Aspect);

	protected:

		VulkanDevice *pDevice;

		VkImage Image;
		VkMemoryRequirements MemoryRequirements;
		VulkanAllocation Allocation;

		VulkanTextureView DefaultView;

		
	};

}