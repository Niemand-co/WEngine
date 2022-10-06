#pragma once

class RHIQueueDescriptor;

class RHIDeviceDescriptor
{
public:
	
	RHIDeviceDescriptor();

	virtual ~RHIDeviceDescriptor();

public:
	
	RHIQueueDescriptor* queueInfos;

	unsigned int queueInfoCount;

	bool enableDebugLayer;

};