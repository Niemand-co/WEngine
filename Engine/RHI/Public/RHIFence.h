#pragma once

class RHIFence : public RHIResource
{
public:

	virtual ~RHIFence() = default;

	virtual void Reset() = 0;

	virtual int32 Wait(double Time = UINT64_MAX) = 0;

};

typedef WEngine::WSharedPtr<RHIFence> WFenceRHIRef;