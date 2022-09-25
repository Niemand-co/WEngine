#pragma once
#include "Render/Descriptor/Public/RHIDescriptor.h"

class RHIDepthStencilDescriptor : public RHIDescriptor
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