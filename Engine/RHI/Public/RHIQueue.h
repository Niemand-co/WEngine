#pragma once

class RHICommandPool;
class RHICommandBuffer;
class RHISwapchain;
class RHISemaphore;

class RHIQueue
{
public:
	
	virtual ~RHIQueue() = default;

	virtual int GetIndex() = 0;

	virtual RHICommandPool* GetCommandPool() = 0;

	virtual void Submit(RHICommandBuffer **cmd, unsigned int count, RHISemaphore *waithSemaphore, RHISemaphore *signalSemaphore) = 0;

	virtual void Present(RHISwapchain *swapchain, unsigned int index, RHISemaphore *semaphore) = 0;

};