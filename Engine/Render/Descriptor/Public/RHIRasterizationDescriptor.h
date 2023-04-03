#pragma once

class RHIRasterizationDescriptor
{
public:

	RHIRasterizationDescriptor() = default;

	~RHIRasterizationDescriptor() = default;

public:

	EPolygonMode polygonMode = EPolygonMode::Triangle;

	float lineWidth = 1.0f;

	EPrimitiveTopology primitivePology = EPrimitiveTopology::TriangleList;

};