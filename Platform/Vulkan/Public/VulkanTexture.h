#pragma once
#include "RHI/Public/RHITexture.h"
#include "Platform/Vulkan/Public/VulkanSurface.h"

namespace Vulkan
{

	class VulkanTextureBase
	{
	public:

		VulkanTextureBase(class VulkanDevice* pInDevice, VkImageCreateInfo* pInfo);

		virtual ~VulkanTextureBase();

		const VulkanSurface& GetSurface() const { return Surface; }

		VkImage GetHandle() const { return Surface.GetImage(); }

		static VulkanTextureBase* Cast(RHITexture* Texture)
		{
			return (VulkanTextureBase*)(Texture->GetTextureRHIBase());
		}

	protected:

		VkMemoryRequirements MemoryRequirements;

		VkDeviceMemory DeviceMemory;

		VulkanDevice *pDevice;

		VulkanSurface Surface;

	};

	class VulkanTexture2D : public RHITexture2D, public VulkanTextureBase
	{
	public:

		VulkanTexture2D(VulkanDevice *pInDevice, VkImageCreateInfo *pInfo);

		virtual ~VulkanTexture2D();

		virtual void* GetTextureRHIBase() override
		{
			VulkanTextureBase *Base = static_cast<VulkanTextureBase*>(this);
			return Base;
		}

	};

	class VulkanTexture2DArray : public RHITexture2DArray, public VulkanTextureBase
	{
	public:

		VulkanTexture2DArray(VulkanDevice *pInDevice, VkImageCreateInfo *pInfo);

		virtual ~VulkanTexture2DArray();

		virtual void* GetTextureRHIBase() override
		{
			VulkanTextureBase* Base = static_cast<VulkanTextureBase*>(this);
			return Base;
		}

	};

	class VulkanTexture3D : public RHITexture3D, public VulkanTextureBase
	{
	public:

		VulkanTexture3D(VulkanDevice *pInDevice, VkImageCreateInfo *pInfo);

		virtual ~VulkanTexture3D();

		virtual void* GetTextureRHIBase() override
		{
			VulkanTextureBase* Base = static_cast<VulkanTextureBase*>(this);
			return Base;
		}

	};

}