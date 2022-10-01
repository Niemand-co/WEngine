#pragma once
#include "RHI/Public/RHIFramebuffer.h"

namespace Vulkan
{

	class VulkanFramebuffer : public RHIFramebuffer
	{
	public:

		VulkanFramebuffer(VkFramebuffer *pFramebuffer, VkDevice *pDevice);

		virtual ~VulkanFramebuffer();

		VkFramebuffer* GetHandle();

	private:

		VkFramebuffer *m_pFramebuffer;

		VkDevice *m_pDevice;

	};

}