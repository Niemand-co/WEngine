#pragma once

class RHIDepthStencilDescriptor
{
public:

	RHIDepthStencilDescriptor();

	~RHIDepthStencilDescriptor() = default;

public:

	bool stencilTestEnabled = false;

	ECompareOP stencilCompareOP = ECompareOP::NotEqual;

	EStencilFailedOP stencilFailedOP = EStencilFailedOP::Keep;

	EStencilFailedOP depthFailedOP = EStencilFailedOP::Keep;

	EStencilFailedOP passOP = EStencilFailedOP::Replace;

	uint32 stencilRef = 1;

	bool depthTestEnabled = true;

	bool depthWriteEnabled = true;

	bool depthBoundsTest = false;

	float maxDepth = 0.0f;

	float minDepth = 1.0f;

	ECompareOP depthCompareOP = ECompareOP::Greater;

};