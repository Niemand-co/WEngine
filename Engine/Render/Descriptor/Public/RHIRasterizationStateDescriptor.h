#pragma once

class RHIRasterizationStateDescriptor
{
public:

	RHIRasterizationStateDescriptor();

	~RHIRasterizationStateDescriptor() = default;

public:

	PolygonMode polygonMode;

	float lineWidth;

	PrimitiveTopology primitivePology;

};