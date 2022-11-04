#pragma once

class RHISemaphore;
class RHIFence;
class RHICommandBuffer;

class RHISubmitDescriptor
{
public:

	RHISubmitDescriptor();

	~RHISubmitDescriptor() = default;

public:

	RHISemaphore **pWaitSemaphores;

	unsigned int waitSemaphoreCount;

	RHISemaphore **pSignalSemaphores;

	unsigned int signalSemaphoreCount;

	unsigned int waitStage;

	unsigned int commandBufferCount;

	RHICommandBuffer **pCommandBuffers;

	RHIFence *pFence;

};