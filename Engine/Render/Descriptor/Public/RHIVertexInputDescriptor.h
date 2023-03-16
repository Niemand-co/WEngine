#pragma once

struct VertexBindingDescription;
struct VertexAttributeDescription;

class RHIVertexInputDescriptor
{
public:

	RHIVertexInputDescriptor() = default;

	~RHIVertexInputDescriptor() = default;

public:

	VertexBindingDescription BindingDescriptions[MaxVertexInputElementCount];

	VertexAttributeDescription AttributeDescriptions[MaxVertexInputElementCount];

	uint32 attributeDescriptionCount;

};