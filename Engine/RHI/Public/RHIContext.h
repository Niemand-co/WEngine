#pragma once
#include "Render/Public/RenderContext.h"

class RHIDevice;
class RHIQueue;
class RHISwapchain;
class RHISurface;
class RHITextureView;
class RHICommandPool;
class RHICommandBuffer;
class RHISemaphore;
class RHIFence;

class RHIContext : public RenderContext
{
public:

	RHIContext(RHIQueue *queue, RHISurface *surface, RHIDevice *device);

	virtual ~RHIContext() = default;

	void Init();

	virtual void RecreateSwapchain();

	virtual RHITextureView* GetTextureView(unsigned int index);

	virtual RHICommandBuffer* GetCommandBuffer();

	virtual int GetNextImage(RHISemaphore* pSignalSemaphore);

	virtual void ExecuteCommandBuffer(RHICommandBuffer *cmd);

	virtual void Submit(RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence* fence);

	virtual bool Present(unsigned int imageIndex, RHISemaphore *semaphore);

protected:

	RHIDevice *m_pDevice;

	RHIQueue *m_pQueue;

	RHISurface *m_pSurface;

	RHISwapchain *m_pSwapchain;

	RHICommandPool *m_pPool;

	std::vector<RHICommandBuffer*> m_pCommandBuffers;

	std::vector<RHITextureView*> m_pTextureViews;

};