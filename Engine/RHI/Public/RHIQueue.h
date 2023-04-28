#pragma once

class RHIQueue : public RHIResource
{
public:

	RHIQueue()
		: RHIResource(ERHIResourceType::RRT_Queue)
	{
	}
	
	virtual ~RHIQueue() = default;

	virtual int GetIndex() = 0;

};