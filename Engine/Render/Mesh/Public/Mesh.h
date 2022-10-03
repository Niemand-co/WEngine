#pragma once

struct Vertex;
struct VertexBindingDescription;
struct VertexAttributeDescription;
class RHIVertexInputDescriptor;

class Mesh
{
public:

	Mesh();

	~Mesh();

	void GenerateVertexInputDescription();

	RHIVertexInputDescriptor GetVertexInputDescriptor();

public:

	Vertex *m_pVertices;

	unsigned int m_vertexCount;

	unsigned int *m_pIndices;

	unsigned int m_indexCount;

	VertexBindingDescription *m_bindingDescription;

	std::vector<VertexAttributeDescription*> m_attributeDescriptions;

};