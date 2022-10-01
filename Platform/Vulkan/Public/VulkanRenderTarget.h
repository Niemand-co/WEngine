#pragma once
#include "RHI/Public/RHIRenderTarget.h"

namespace Vulkan
{

	class VulkanRenderTarget : public RHIRenderTarget
	{
	public:

		VulkanRenderTarget(VkFramebuffer *framebuffer, unsigned int width, unsigned int height, VkDevice *device);

		virtual ~VulkanRenderTarget();

		VkFramebuffer* GetHandle();

	private:

		VkFramebuffer *m_framebuffer;

		VkDevice *m_pDevice;

	};

}