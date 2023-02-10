#pragma once
#include "RHI/Public/RHISemaphore.h"

namespace Vulkan
{

	class VulkanSemaphore : public RHISemaphore
	{
	public:

		VulkanSemaphore(class VulkanDevice *pInDevice);

		virtual ~VulkanSemaphore();

		VkSemaphore GetHandle() const { return Semaphore; }

	private:

		VkSemaphore Semaphore;

		VulkanDevice *pDevice;

};

}