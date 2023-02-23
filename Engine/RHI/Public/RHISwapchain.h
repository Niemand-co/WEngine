#pragma once

class RHITexture;
class RHISemaphore;
class RHIFence;

class RHISwapchain
{
public:

	virtual ~RHISwapchain() = default;

protected:

	WEngine::WArray<class WRenderTarget*> RenderTargets;

	WEngine::WArray<RHISemaphore*> ImageAcquireSemaphore;

	WEngine::WArray<RHIFence*> ImageAcquireFence;

};