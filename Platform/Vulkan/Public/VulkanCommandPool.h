#pragma once
#include "RHI/Public/RHICommandPool.h"

namespace Vulkan
{

	class VulkanCommandPool : public RHICommandPool
	{
	public:

		VulkanCommandPool(VkCommandPool *commandPool, VkDevice *device);

		virtual ~VulkanCommandPool();

		virtual RHICommandBuffer* GetCommandBuffer(bool isPrimary = true) override;

		virtual WEngine::WArray<RHICommandBuffer*> GetCommandBuffer(unsigned int count, bool isPrimary = true) override;

	private:

		VkCommandPool *m_commandPool;

		VkDevice *m_device;

	};

}