#pragma once
#include "RHI/Public/RHISwapchain.h"
#include "Platform/Vulkan/Public/VulkanSemaphore.h"
#include "Platform/Vulkan/Public/VulkanFence.h"

namespace Vulkan
{

	class VulkanSwapchain : public RHISwapchain, public VulkanResource
	{
	public:

		VulkanSwapchain(class VulkanDevice *pInDevice, class VulkanInstance *pInInstance, VkSwapchainCreateInfoKHR* pInfo);

		virtual ~VulkanSwapchain();

		virtual int32 AcquireImageIndex(RHISemaphore** OutSemaphore) override;

		virtual int32 Present(RHIQueue *Queue, RHISemaphore *RenderingDoneSemaphore) override;

		VkSwapchainKHR GetHandle() const { return Swapchain; }

	private:

		VulkanDevice *pDevice;

		VulkanInstance *pInstance;

		VkSwapchainKHR Swapchain;

		VkSurfaceKHR Surface;

		VkSwapchainCreateInfoKHR RecreateInfo;

		VkSurfaceCapabilitiesKHR SurfaceCapabilities;

		WEngine::WArray<WSemaphoreRHIRef> ImageAcquireSemaphore;

		WEngine::WArray<WFenceRHIRef> ImageAcquireFence;

		uint32 PresentID;

		uint32 SemaphoreID;

		int32 CurrentImageIndex;

	};

}