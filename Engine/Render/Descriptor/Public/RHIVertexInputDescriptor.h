#pragma once

struct VertexBindingDescription;
struct VertexAttributeDescription;

class RHIVertexInputDescriptor
{
public:

	RHIVertexInputDescriptor() = default;

	~RHIVertexInputDescriptor() = default;

	void Generate(WEngine::WArray<class VertexInputElement>& Elements);

public:

	VertexBindingDescription BindingDescriptions[MaxVertexInputElementCount];

	VertexAttributeDescription AttributeDescriptions[MaxVertexInputElementCount];

	uint32 attributeDescriptionCount;

};