#pragma once

class RHISemaphore : public RHIResource
{
public:

	virtual ~RHISemaphore() = default;
};

typedef RHISemaphore* WSemaphoreRHIRef;