#pragma once

class RHITexture;
class RHISemaphore;
class RHIFence;

class RHISwapchain
{
public:

	virtual ~RHISwapchain() = default;

	virtual RHITexture* GetTexture(unsigned int index);

	virtual int32 AcquireImageIndex(RHISemaphore** outSemaphore) = 0;

	virtual void Present(class RHIQueue *queue, RHISemaphore *renderingDoneSemaphore) = 0;

protected:

	WEngine::WArray<class WRenderTarget*> RenderTargets;

	WEngine::WArray<RHISemaphore*> ImageAcquireSemaphore;

	WEngine::WArray<RHIFence*> ImageAcquireFence;

};