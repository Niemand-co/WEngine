#pragma once
#include "RHI/Public/RHIQueue.h"

enum class RHIQueueType;

namespace Vulkan
{

	class VulkanQueue : public RHIQueue
	{
	public:

		VulkanQueue(VkQueue *queue, int queueFamilyIndex, VkDevice *device);

		virtual ~VulkanQueue();

		virtual int GetIndex() override;

		virtual RHICommandPool* GetCommandPool() override;

		virtual void Submit(RHISubmitDescriptor* descriptor) override;

		virtual bool Present(RHISwapchain *swapchain, unsigned int index, RHISemaphore *semaphore) override;

		VkQueue* GetHandle();

	private:

		VkQueue *m_queue;

		VkDevice *m_device;

		RHIQueueType m_type;

		int m_queueFamilyIndex;

	};

}