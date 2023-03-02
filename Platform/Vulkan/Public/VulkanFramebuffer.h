#pragma once
#include "RHI/Public/RHIFramebuffer.h"

namespace Vulkan
{

	class VulkanFramebuffer : public RHIFramebuffer
	{
	public:

		VulkanFramebuffer(class VulkanDevice *pInDevice, VkFramebufferCreateInfo *pInfo);

		virtual ~VulkanFramebuffer();

		VkFramebuffer GetHandle() const { return Framebuffer; }

	private:

		VulkanDevice *pDevice;

		VkFramebuffer Framebuffer;

	};

}