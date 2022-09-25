#pragma once
#include "RHI/Public/RHIRenderTarget.h"

class VulkanRenderTarget : public RHIRenderTarget
{
public:

	VulkanRenderTarget(VkFramebuffer *framebuffer, unsigned int width, unsigned int height);

	virtual ~VulkanRenderTarget();

	VkFramebuffer* GetHandle();

private:

	VkFramebuffer *m_framebuffer;

};