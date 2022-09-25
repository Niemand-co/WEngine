#pragma once
#include "Render/Descriptor/Public/RHIDescriptor.h"

class RHIQueueDescriptor : public RHIDescriptor
{
public:

	RHIQueueDescriptor();

	virtual ~RHIQueueDescriptor();

public:
	
	unsigned int count;

	RHIQueueType type;
};