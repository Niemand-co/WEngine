#pragma once

class RHIQueueDescriptor
{
public:

	RHIQueueDescriptor();

	~RHIQueueDescriptor() = default;

public:
	
	unsigned int count;

	RHIQueueType type;
};