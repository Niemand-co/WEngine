#pragma once

class RHIViewportDescriptor
{
public:

	RHIViewportDescriptor() = default;

	~RHIViewportDescriptor() = default;

public:

    float    x = 0.0f;

    float    y = 0.0f;

    float    width = 0.0f;

    float    height = 0.0f;

    float    minDepth = 0.0f;

    float    maxDepth = 1.0f;

};