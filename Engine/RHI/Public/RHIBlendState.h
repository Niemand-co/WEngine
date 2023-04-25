#pragma once

class RHIAttachmentBlendState : public RHIResource
{
public:

	RHIAttachmentBlendState()
		: RHIResource(ERHIResourceType::RRT_State)
	{
	}

	virtual ~RHIAttachmentBlendState() = default;

};

class RHIBlendState : public RHIResource
{
public:

	RHIBlendState()
		: RHIResource(ERHIResourceType::RRT_State)
	{
	}

	virtual ~RHIBlendState() = default;

	virtual void SetAttachmentBlendState(uint32, RHIAttachmentBlendState *InState) = 0;

};