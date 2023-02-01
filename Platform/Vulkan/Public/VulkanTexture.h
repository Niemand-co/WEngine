#pragma once
#include "RHI/Public/RHITexture.h"

class RHIDevice;

namespace Vulkan
{

	class VulkanTextureBase
	{
	public:

		VulkanTextureBase();

		virtual ~VulkanTextureBase();

	private:

		VkImage *pImage;

		VkImageView *pImageView;

		VkMemoryRequirements *pMemoryRequirements;

		VkDeviceMemory *pDeviceMemory;

	};

	class VulkanTexture2D : public RHITexture2D, public VulkanTextureBase
	{
	public:

		VulkanTexture2D();

		virtual ~VulkanTexture2D();



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