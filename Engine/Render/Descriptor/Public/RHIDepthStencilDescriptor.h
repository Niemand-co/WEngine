#pragma once

class RHIDepthStencilDescriptor
{
public:

	RHIDepthStencilDescriptor();

	~RHIDepthStencilDescriptor() = default;

public:

	bool stencilTestEnabled;

	CompareOP stencilCompareOP;

	StencilFailedOP stencilFailedOP;

	StencilFailedOP depthFailedOP;

	StencilFailedOP passOP;

	unsigned int stencilRef;

	bool depthTestEnabled;

	bool depthWriteEnabled;

	bool depthBoundsTest;

	float maxDepth;

	float minDepth;

	CompareOP depthCompareOP;

};