#pragma once

class RHISemaphore : public RHIResource
{
public:

	RHISemaphore()
		: RHIResource(ERHIResourceType::RRT_Semaphore)
	{
	}

	virtual ~RHISemaphore() = default;

};

typedef RHISemaphore* WSemaphoreRHIRef;