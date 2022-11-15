#pragma once

struct Vertex;

class Mesh
{
public:

	Mesh();

	~Mesh();

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

	static Mesh* GetPlane();

public:

	Vertex *m_pVertices;

	unsigned int m_vertexCount;

	unsigned int *m_pIndices;

	unsigned int m_indexCount;

private:

	VertexBindingDescription *m_bindingDescription;

	std::vector<VertexAttributeDescription*> m_attributeDescriptions;

};

class MeshLibrary
{
public:

	MeshLibrary();

	~MeshLibrary();

private:

	std::vector<Mesh*> m_pMeshes;

};