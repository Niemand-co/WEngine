#pragma once
#include "RHI/Public/RHISwapchain.h"

namespace Vulkan
{

	class VulkanSwapchain : public RHISwapchain
	{
	public:

		VulkanSwapchain(VkSwapchainKHR *swapchain, VkImage* images, const unsigned int& imageCount, VkDevice *device, unsigned int familyIndex);

		virtual ~VulkanSwapchain();

		void SetHandle(VkSwapchainKHR *swapchain);

		VkSwapchainKHR* GetHandle();

		unsigned int GetQueueFamilyIndex();

	private:

		VkSwapchainKHR *m_swapchain;

		VkDevice *m_device;

		unsigned int m_familyIndex;

	};

}