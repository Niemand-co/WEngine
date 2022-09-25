#pragma once
#include "RHI/Public/RHICommandPool.h"

namespace Vulkan
{

	class VulkanCommandPool : public RHICommandPool
	{
	public:

		VulkanCommandPool(VkCommandPool *commandPool, VkDevice *device);

		virtual ~VulkanCommandPool();

		virtual RHICommandBuffer* GetCommandBuffer(unsigned int num = 1) override;

	private:

		VkCommandPool *m_commandPool;

		VkDevice *m_device;

	};

}