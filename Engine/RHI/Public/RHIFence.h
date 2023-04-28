#pragma once

class RHIFence : public RHIResource
{
public:

	RHIFence()
		: RHIResource(ERHIResourceType::RRT_Fence)
	{
	}

	virtual ~RHIFence() = default;

	virtual void Reset() = 0;

	virtual bool Wait(double Time = UINT64_MAX) = 0;

};

typedef WEngine::WSharedPtr<RHIFence> WFenceRHIRef;