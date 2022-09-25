#pragma once
#include "RHI/Public/RHIRenderPass.h"

namespace Vulkan
{

	class VulkanRenderPass : public RHIRenderPass
	{
	public:

		VulkanRenderPass(VkRenderPass *renderPass);

		virtual ~VulkanRenderPass();

		VkRenderPass* GetHandle();

	private:

		VkRenderPass *m_renderPass;

	};

}