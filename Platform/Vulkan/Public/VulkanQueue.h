#pragma once
#include "RHI/Public/RHIQueue.h"

class RHICommandPool;

namespace Vulkan
{

	class VulkanQueue : public RHIQueue
	{
	public:

		VulkanQueue(class VulkanDevice* pInDevice, uint32 inQueueFamilyIndex);

		virtual ~VulkanQueue();

		virtual int32 GetIndex() override;

		RHICommandPool* GetCommandPool(class VulkanCommandBufferManager* pInManager);

		void Submit(class VulkanCommandBuffer* CmdBuffer, uint32 NumSignalSemaphore = 0u, VkSemaphore* pSignalSemaphores = nullptr);

		VulkanCommandBuffer* GetLastSubmittedCmdBuffer() const { return LastSubmittedCmdBuffer; }

		VkQueue GetHandle() const { return Queue; }

	private:

		VkQueue Queue;

		VulkanDevice* pDevice;

		RHIQueueType m_type;

		uint32 QueueFamilyIndex;

		VulkanCommandBuffer *LastSubmittedCmdBuffer = nullptr;

	};

}