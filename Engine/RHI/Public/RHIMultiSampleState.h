#pragma once

class RHIMultiSampleState : public RHIResource
{
public:

	RHIMultiSampleState()
		: RHIResource(ERHIResourceType::RRT_State)
	{
	}

	virtual ~RHIMultiSampleState() = default;

};