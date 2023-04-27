#pragma once
#include "RHI/Public/RHIRenderPass.h"

namespace Vulkan
{

	class VulkanRenderPass : public RHIRenderPass
	{
	public:

		VulkanRenderPass(class VulkanDevice *pInDevice, VkRenderPassCreateInfo *pInfo);
		VulkanRenderPass(VulkanDevice *pInDevice, const class VulkanRenderTargetLayout& InRTLayout);

		virtual ~VulkanRenderPass();

		void Begin(class VulkanCommandBuffer* CmdBuffer);

		void End(class VulkanCommandBuffer *CmdBuffer);

		VkRenderPass GetHandle() const { return RenderPass; }

	private:
		
		VulkanDevice *pDevice;

		VkRenderPass RenderPass;

	};

}