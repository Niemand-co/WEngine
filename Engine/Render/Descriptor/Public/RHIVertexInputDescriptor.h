#pragma once
#include "Render/Descriptor/Public/RHIDescriptor.h"

struct VertexBindingDescription;
struct VertexAttributeDescription;

class RHIVertexInputDescriptor : RHIDescriptor
{
public:

	RHIVertexInputDescriptor();

	virtual ~RHIVertexInputDescriptor();

public:

	VertexBindingDescription *bindingDescription;

	VertexAttributeDescription **pAttributeDescription;

	unsigned int attributeDescriptionCount;

};