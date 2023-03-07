#pragma once

class RHIDepthStencilDescriptor
{
public:

	RHIDepthStencilDescriptor();

	~RHIDepthStencilDescriptor() = default;

public:

	bool stencilTestEnabled = false;

	CompareOP stencilCompareOP = CompareOP::NotEqual;

	StencilFailedOP stencilFailedOP = StencilFailedOP::Keep;

	StencilFailedOP depthFailedOP = StencilFailedOP::Keep;

	StencilFailedOP passOP = StencilFailedOP::Replace;

	uint32 stencilRef = 1;

	bool depthTestEnabled = true;

	bool depthWriteEnabled = true;

	bool depthBoundsTest = false;

	float maxDepth = 0.0f;

	float minDepth = 1.0f;

	CompareOP depthCompareOP = CompareOP::Greater;

};