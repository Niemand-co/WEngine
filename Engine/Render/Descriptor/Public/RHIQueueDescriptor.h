#pragma once

class RHIQueueDescriptor
{
public:

	RHIQueueDescriptor();

	virtual ~RHIQueueDescriptor();

public:
	
	unsigned int count;

	RHIQueueType type;
};