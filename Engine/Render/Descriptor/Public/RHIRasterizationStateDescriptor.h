#pragma once

class RHIRasterizationStateDescriptor
{
public:

	RHIRasterizationStateDescriptor() = default;

	~RHIRasterizationStateDescriptor() = default;

public:

	PolygonMode polygonMode = PolygonMode::Triangle;

	float lineWidth = 1.0f;

	PrimitiveTopology primitivePology = PrimitiveTopology::TriangleList;

};