#pragma once
#include "RHI/Public/RHITexture.h"

class RHIDevice;

namespace Vulkan
{

	class VulkanTexture : public RHITexture
	{
	public:

		VulkanTexture(VkImage *image, VkMemoryRequirements* pMemoryRequirements, VkDeviceMemory* pDeviceMemory, VkDevice *device, bool isDisplay = false);

		virtual ~VulkanTexture();

		virtual RHITextureView* CreateTextureView(RHITextureViewDescriptor *descriptor) override;

		virtual void LoadData(std::string path, RHIContext* context) override;

		VkImage* GetHandle();

		void SetHandle(VkImage *image);

	private:

		VkImage *m_pImage;

		VkDevice *m_pDevice;

		bool m_isDisplayTexture;

		VkMemoryRequirements *m_pMemoryRequirements;

		VkDeviceMemory *m_pDeviceMemory;

	};

}