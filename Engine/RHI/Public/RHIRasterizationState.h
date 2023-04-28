#pragma once

class RHIRasterizationState : public RHIResource
{
public:

	RHIRasterizationState()
		: RHIResource(ERHIResourceType::RRT_State)
	{
	}

	virtual ~RHIRasterizationState() = default;

};