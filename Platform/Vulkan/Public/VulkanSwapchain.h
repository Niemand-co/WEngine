#pragma once
#include "RHI/Public/RHISwapchain.h"

namespace Vulkan
{

	class VulkanSwapchain : public RHISwapchain
	{
	public:

		VulkanSwapchain(VkSwapchainKHR *swapchain, VkImage* images, const unsigned int& imageCount, VkDevice *device);

		virtual ~VulkanSwapchain();

		VkSwapchainKHR* GetHandle();

	private:

		VkSwapchainKHR *m_swapchain;

		VkDevice *m_device;

	};

}