#pragma once
#include "RHI/Public/RHIViewport.h"

namespace Vulkan
{

	class VulkanViewport : public RHIViewport
	{
	public:

		VulkanViewport(class VulkanDevice* pInDevice);

		virtual ~VulkanViewport();

		virtual void Tick() override;

		virtual uint32 AcquireImageIndex() override;

		virtual void Present() override;

	private:

		void CreateSwapchain(class RHISwapchainDescriptor *descriptor);

		void DestroySwapchain();

		void RecreateSwapchain(class RHISwapchainDescriptor* descriptor);

	private:

		VulkanDevice *pDevice;

		class VulkanSwapchain *pSwapchain;

		class VulkanSemaphore* pAcquireImageSemaphore;

		WEngine::WArray<VkImage> BackBufferImages;

		WEngine::WArray<VulkanSemaphore*> RenderingDoneSemaphores;

		WEngine::WArray<class VulkanTextureView> TextureViews;

	};

}