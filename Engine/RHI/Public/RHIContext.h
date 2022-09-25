#pragma once
#include "Render/Public/RenderContext.h"

class RHIInstance;
class RHIDevice;
class RHIQueue;
class RHISwapchain;
class RHICommandPool;
class RHICommandBuffer;
class RHISemaphore;

class RHIContext : public RenderContext
{
public:

	RHIContext(RHIQueue *queue, RHIDevice *device);

	virtual ~RHIContext() = default;

	void Init();

	virtual RHICommandBuffer* GetCommandBuffer();

	virtual void ExecuteCommandBuffer(RHICommandBuffer *cmd);

	virtual void Submit(RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore);

	virtual void Present(unsigned int imageIndex, RHISwapchain *swapchain, RHISemaphore *semaphore);

protected:

	RHIDevice *m_pDevice;

	RHIQueue *m_pQueue;

	RHICommandPool *m_pPool;

	std::vector<RHICommandBuffer*> m_pCommandBuffers;

};