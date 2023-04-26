#pragma once

class RHISamplerState : public RHIResource
{
public:

	RHISamplerState()
		: RHIResource(ERHIResourceType::RRT_SamplerState)
	{
	}

	virtual bool IsImmutable() const { return false; }

};