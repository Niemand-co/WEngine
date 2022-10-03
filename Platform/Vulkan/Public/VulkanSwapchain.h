#pragma once
#include "RHI/Public/RHISwapchain.h"

namespace Vulkan
{

	class VulkanSwapchain : public RHISwapchain
	{
	public:

		VulkanSwapchain(VkSwapchainKHR *swapchain, VkDevice *device, unsigned int familyIndex);

		virtual ~VulkanSwapchain();

		void SetHandle(VkSwapchainKHR *swapchain);

		VkSwapchainKHR* GetHandle();

		unsigned int GetQueueFamilyIndex();

	private:

		VkSwapchainKHR *m_pSwapchain;

		VkDevice *m_pDevice;

		unsigned int m_familyIndex;

	};

}