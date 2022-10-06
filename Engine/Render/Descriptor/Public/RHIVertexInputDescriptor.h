#pragma once

struct VertexBindingDescription;
struct VertexAttributeDescription;

class RHIVertexInputDescriptor
{
public:

	RHIVertexInputDescriptor();

	virtual ~RHIVertexInputDescriptor();

public:

	VertexBindingDescription *bindingDescription;

	VertexAttributeDescription **pAttributeDescription;

	unsigned int attributeDescriptionCount;

};