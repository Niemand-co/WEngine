#pragma once

class RHIRasterizationDescriptor
{
public:

	RHIRasterizationDescriptor() = default;

	~RHIRasterizationDescriptor() = default;

public:

	PolygonMode polygonMode = PolygonMode::Triangle;

	float lineWidth = 1.0f;

	PrimitiveTopology primitivePology = PrimitiveTopology::TriangleList;

};