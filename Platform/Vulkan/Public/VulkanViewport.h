#pragma once
#include "RHI/Public/RHIViewport.h"
#include "HAL/Public/WScopeLock.h"

class RHISwapchainDescriptor;
class RHIViewportDescriptor;

namespace Vulkan
{

	class VulkanViewport : public RHIViewport
	{
	public:

		VulkanViewport(class VulkanDevice* pInDevice, RHIViewportDescriptor *descriptor);

		virtual ~VulkanViewport();

		virtual void Tick() override;

		virtual bool AcquireImageIndex() override;

		virtual int32 Present(RHICommandBuffer *CmdBuffer, RHIQueue *Queue) override;

		void WaitForFrameEvenCompletion();

	private:

		void CreateSwapchain();

		void DestroySwapchain();

		void RecreateSwapchain();

	private:

		VulkanDevice *pDevice;

		class VulkanSwapchain *pSwapchain;

		WEngine::WSharedPtr<class VulkanSemaphore> pAcquireImageSemaphore;

		WEngine::WArray<VkImage> BackBufferImages;

		WEngine::WArray<WEngine::WSharedPtr<VulkanSemaphore>> RenderingDoneSemaphores;

		WEngine::WArray<class VulkanTextureView*> TextureViews;

		WEngine::WSharedPtr<class VulkanTexture2D> RenderingBackBuffer;

		uint32 Width;

		uint32 Height;

		Format PixelFormat;

		int32 AcquiredImageIndex;

		WCriticalSection SwapchainRecreationSection;

		class VulkanCommandBuffer *LastFrameCmdBuffer = nullptr;

	};

}