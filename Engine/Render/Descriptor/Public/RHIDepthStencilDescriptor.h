#pragma once

class RHIDepthStencilDescriptor
{
public:

	RHIDepthStencilDescriptor();

	virtual ~RHIDepthStencilDescriptor();

public:

	bool depthTestEnabled;

	bool depthWriteEnabled;

	bool depthBoundsTest;

	float maxDepth;

	float minDepth;

	DepthCompareOP depthCompareOP;

};