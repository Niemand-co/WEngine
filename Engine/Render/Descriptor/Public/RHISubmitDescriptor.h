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

	RHISemaphore **pSignalSemaphores;

	unsigned int signalSemaphoreCount;

	RHICommandBuffer *pCommandBuffer;

};