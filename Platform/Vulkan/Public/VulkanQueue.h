#pragma once
#include "RHI/Public/RHIQueue.h"

namespace Vulkan
{

	class VulkanQueue : public RHIQueue
	{
	public:

		VulkanQueue(class VulkanDevice* pInDevice, uint32 inQueueFamilyIndex);

		virtual ~VulkanQueue();

		virtual int32 GetIndex() override;

		virtual class RHICommandPool* GetCommandPool() override;

		void Submit(class VulkanCommandBuffer* CmdBuffer, WEngine::WArray<VkSemaphore>& SignalSemaphores);

		VkQueue GetHandle() const { return Queue; }

	private:

		VkQueue Queue;

		VulkanDevice* pDevice;

		RHIQueueType m_type;

		uint32 QueueFamilyIndex;

	};

}