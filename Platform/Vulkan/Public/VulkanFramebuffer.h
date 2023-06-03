#pragma once
#include "RHI/Public/RHIFramebuffer.h"

class VulkanRenderTargetLayout;
class RHIFramebufferDescriptor;

namespace Vulkan
{

	class VulkanFramebuffer : public RHIFramebuffer
	{
	public:

		VulkanFramebuffer(class VulkanDevice *pInDevice, const RHIFramebufferDescriptor* RTInfo, const VulkanRenderTargetLayout& RTLayout, class VulkanRenderPass* RenderPass);

		virtual ~VulkanFramebuffer();

		VkFramebuffer GetHandle() const { return Framebuffer; }

		const VkRect2D& GetRenderArea() const { return RenderArea; }

		bool MatchInfo(const RHIFramebufferDescriptor* RTInfo);

	private:

		VulkanDevice *pDevice;

		VkFramebuffer Framebuffer;

		VkRect2D RenderArea;

		uint32 NumAttachments;

		VkImageView Attachments[MaxSimultaneousRenderTargets + 1];

		uint32 NumColorAttachments;
		uint8 bHasDepthStencilAttachment;
		VkImage ColorAttachments[MaxSimultaneousRenderTargets];
		VkImage DepthStencilAttachment;

		FClearValue ClearValues[MaxSimultaneousRenderTargets + 1];
	};

}