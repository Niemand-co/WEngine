#pragma once
#include "RHI/Public/RHITexture.h"

class RHIDevice;

namespace Vulkan
{

	class VulkanTextureBase
	{
	public:

		VulkanTextureBase(VkImage* pInImage);

		virtual ~VulkanTextureBase();

		VkImage* GetHandle() const { return pImage; }

	private:

		VkMemoryRequirements *pMemoryRequirements;

		VkDeviceMemory *pDeviceMemory;

		VkImage *pImage;

	};

	class VulkanTexture2D : public RHITexture2D, public VulkanTextureBase
	{
	public:

		VulkanTexture2D(VkImage *pImage, uint32 width, uint32 height);

		virtual ~VulkanTexture2D();

		virtual RHITextureView* CreateTextureView(RHITextureViewDescriptor* descriptor) override;

	};

	class VulkanTexture3D : public RHITexture3D
	{

	};

	class VulkanTexture2DArray : public RHITexture2DArray
	{

	};

	class VulkanTexture3DArray : public RHITexture3DArray
	{

	};

}