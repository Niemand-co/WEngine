#pragma once
#include "RHI/Public/RHITexture.h"

namespace Vulkan
{

	class VulkanTextureBase
	{
	public:

		VulkanTextureBase(VulkanDevice* pInDevice, VkImageCreateInfo* pInfo);

		virtual ~VulkanTextureBase();

		VkImage GetHandle() const { return Image; }

	protected:

		VkMemoryRequirements MemoryRequirements;

		VkDeviceMemory DeviceMemory;

		VkImage Image;

		class VulkanDevice *pDevice;

	};

	class VulkanTexture2D : public RHITexture2D, public VulkanTextureBase
	{
	public:

		VulkanTexture2D(VulkanDevice *pInDevice, VkImageCreateInfo *pInfo);

		virtual ~VulkanTexture2D();

	};

	class VulkanTexture2DArray : public RHITexture2DArray, public VulkanTextureBase
	{
	public:

		VulkanTexture2DArray(VulkanDevice *pInDevice, VkImageCreateInfo *pInfo);

		virtual ~VulkanTexture2DArray();

	};

	class VulkanTexture3D : public RHITexture3D, public VulkanTextureBase
	{
	public:

		VulkanTexture3D(VulkanDevice *pInDevice, VkImageCreateInfo *pInfo);

		virtual ~VulkanTexture3D();

	};

}