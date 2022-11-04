#pragma once

struct VertexBindingDescription;
struct VertexAttributeDescription;

class RHIVertexInputDescriptor
{
public:

	RHIVertexInputDescriptor();

	~RHIVertexInputDescriptor() = default;

public:

	VertexBindingDescription *bindingDescription;

	VertexAttributeDescription **pAttributeDescription;

	unsigned int attributeDescriptionCount;

};