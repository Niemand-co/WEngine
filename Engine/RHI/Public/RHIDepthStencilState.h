#pragma once

class RHIDepthStencilState : public RHIResource
{
public:

	RHIDepthStencilState()
		: RHIResource(ERHIResourceType::RRT_State)
	{
	}

	virtual ~RHIDepthStencilState() = default;

};