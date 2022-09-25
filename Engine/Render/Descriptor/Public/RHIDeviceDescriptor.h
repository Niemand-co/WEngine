#pragma once
#include "Render/Descriptor/Public/RHIDescriptor.h"

class RHIQueueDescriptor;

class RHIDeviceDescriptor : public RHIDescriptor
{
public:
	
	RHIDeviceDescriptor();

	virtual ~RHIDeviceDescriptor();

public:
	
	RHIQueueDescriptor* queueInfos;

	unsigned int queueInfoCount;

	bool enableDebugLayer;

};