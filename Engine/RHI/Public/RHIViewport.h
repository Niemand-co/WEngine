#pragma once

class RHIViewport : public RHIResource
{
public:

	virtual ~RHIViewport();

	virtual void Tick() = 0;

	virtual uint32 AcquireImageIndex() = 0;

	virtual void Present() = 0;

};