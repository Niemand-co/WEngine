#pragma once

class RHICommandPool;
class RHICommandBuffer;
class RHISwapchain;
class RHISemaphore;
class RHIFence;

class RHIQueue
{
public:
	
	virtual ~RHIQueue() = default;

	virtual int GetIndex() = 0;

	virtual RHICommandPool* GetCommandPool() = 0;

	virtual void Submit(RHICommandBuffer **cmd, unsigned int count, RHISemaphore *waithSemaphore, RHISemaphore *signalSemaphore, RHIFence *fence) = 0;

	virtual bool Present(RHISwapchain *swapchain, unsigned int index, RHISemaphore *semaphore) = 0;

};