#pragma once
#include "RHI/Public/RHICommandPool.h"

namespace Vulkan
{

	class VulkanCommandPool : public RHICommandPool
	{
	public:

		VulkanCommandPool(class VulkanDevice* pInDevice, VkCommandPoolCreateInfo* pInfo);

		virtual ~VulkanCommandPool();

		virtual RHICommandBuffer* GetCommandBuffer() override;

	private:

		VkCommandPool CommandPool;

		VulkanDevice *pDevice;

	};

}