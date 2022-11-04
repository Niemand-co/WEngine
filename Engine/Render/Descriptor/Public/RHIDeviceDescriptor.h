#pragma once

class RHIQueueDescriptor;

class RHIDeviceDescriptor
{
public:
	
	RHIDeviceDescriptor();

	~RHIDeviceDescriptor() = default;

public:
	
	RHIQueueDescriptor* queueInfos;

	unsigned int queueInfoCount;

	bool enableDebugLayer;

};