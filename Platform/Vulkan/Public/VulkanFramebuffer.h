#pragma once
#include "RHI/Public/RHIFramebuffer.h"

class VulkanFramebuffer : public RHIFramebuffer
{
public:

	VulkanFramebuffer(VkFramebuffer *framebuffer);

	virtual ~VulkanFramebuffer();

	VkFramebuffer* GetHandle();

private:

	VkFramebuffer *m_framebuffer;

};