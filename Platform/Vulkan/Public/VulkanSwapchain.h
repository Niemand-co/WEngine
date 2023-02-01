#pragma once
#include "RHI/Public/RHISwapchain.h"

namespace Vulkan
{

	class VulkanSwapchain : public RHISwapchain, public VulkanResource
	{
	public:

		VulkanSwapchain(VkSwapchainKHR *swapchain, VkDevice *device, unsigned int familyIndex);

		virtual ~VulkanSwapchain();

		virtual int32 AcquireImageIndex(RHISemaphore** outSemaphore) override;

		virtual void Present(RHIQueue *queue, RHISemaphore *renderingDoneSemaphore) override;

		void SetHandle(VkSwapchainKHR *swapchain);

		VkSwapchainKHR* GetHandle();

		unsigned int GetQueueFamilyIndex();

	private:

		VkDevice *pDevice;

		VkSwapchainKHR *pSwapchain;

		VkSurfaceKHR *pSurface;

		VkDevice *m_pDevice;

		unsigned int m_familyIndex;

	};

}