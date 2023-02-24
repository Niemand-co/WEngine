#pragma once
#include "RHI/Public/RHIViewport.h"
#include "HAL/Public/WScopeLock.h"

class RHISwapchainDescriptor;

namespace Vulkan
{

	class VulkanViewport : public RHIViewport
	{
	public:

		VulkanViewport(class VulkanDevice* pInDevice);

		virtual ~VulkanViewport();

		virtual void Tick() override;

		virtual bool AcquireImageIndex() override;

		virtual int32 Present(RHICommandBuffer *CmdBuffer, RHIQueue *Queue) override;

	private:

		void CreateSwapchain(RHISwapchainDescriptor *descriptor);

		void DestroySwapchain();

		void RecreateSwapchain();

	private:

		VulkanDevice *pDevice;

		class VulkanSwapchain *pSwapchain;

		class VulkanSemaphore* pAcquireImageSemaphore;

		WEngine::WArray<VkImage> BackBufferImages;

		WEngine::WArray<VulkanSemaphore*> RenderingDoneSemaphores;

		WEngine::WArray<class VulkanTextureView*> TextureViews;

		Format PixelFormat;

		int32 AcquiredImageIndex;

		WCriticalSection SwapchainRecreationSection;

	};

}