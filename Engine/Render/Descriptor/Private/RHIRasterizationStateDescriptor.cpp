#include "pch.h"
#include "Render/Descriptor/Public/RHIRasterizationStateDescriptor.h"

RHIRasterizationStateDescriptor::RHIRasterizationStateDescriptor()
{
	polygonMode = PolygonMode::Triangle;
	lineWidth = 1.0f;
	primitivePology = PrimitiveTopology::TriangleList;
}
