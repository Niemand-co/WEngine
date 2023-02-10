#pragma once

class RHISemaphore : public RHIResource
{
public:

	virtual ~RHISemaphore() = default;
};

typedef WEngine::WSharedPtr<RHISemaphore> WSemaphoreRHIRef;