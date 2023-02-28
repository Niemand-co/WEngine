#pragma once
#include "RHI/Public/RHICommandPool.h"

namespace Vulkan
{

	class VulkanCommandPool : public RHICommandPool
	{
	public:

		VulkanCommandPool(class VulkanDevice* pInDevice, VkCommandPoolCreateInfo* pInfo, class VulkanCommandBufferManager* pInManager);

		virtual ~VulkanCommandPool();

		virtual RHICommandBuffer* GetCommandBuffer() override;

		class VulkanCommandBuffer* AllocateCmdBuffer();

		VulkanCommandBufferManager* GetManager() const { return pManager; }

		VkCommandPool GetHandle() const { return CommandPool; }

	private:

		VkCommandPool CommandPool;

		VulkanCommandBufferManager *pManager;

		WCriticalSection CS;

		VulkanDevice *pDevice;

		WEngine::WArray<VulkanCommandBuffer*> CmdBuffers;

		friend class VulkanCommandBufferManager;

	};

}