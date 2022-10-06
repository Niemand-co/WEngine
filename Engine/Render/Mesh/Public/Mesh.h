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

	void* operator new(size_t size)
	{
		return WEngine::Allocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		WEngine::Allocator::Get()->Deallocate(pData);
	}

public:

	static Mesh* GetCube();

public:

	Vertex *m_pVertices;

	unsigned int m_vertexCount;

	unsigned int *m_pIndices;

	unsigned int m_indexCount;

private:

	VertexBindingDescription *m_bindingDescription;

	std::vector<VertexAttributeDescription*> m_attributeDescriptions;

};