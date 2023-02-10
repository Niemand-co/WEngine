#pragma once
#include "RHI/Public/RHICommandPool.h"

namespace Vulkan
{

	class VulkanCommandPool : public RHICommandPool
	{
	public:

		VulkanCommandPool(class VulkanDevice* pInDevice, VkCommandPoolCreateInfo* pInfo);

		virtual ~VulkanCommandPool();

		virtual class RHICommandBuffer* GetCommandBuffer(bool bPrimary = true) override;

	private:

		VkCommandPool CommandPool;

		VulkanDevice *pDevice;

	};

}