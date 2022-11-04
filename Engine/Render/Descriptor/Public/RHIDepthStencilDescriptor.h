#pragma once

class RHIDepthStencilDescriptor
{
public:

	RHIDepthStencilDescriptor();

	~RHIDepthStencilDescriptor() = default;

public:

	bool depthTestEnabled;

	bool depthWriteEnabled;

	bool depthBoundsTest;

	float maxDepth;

	float minDepth;

	DepthCompareOP depthCompareOP;

};