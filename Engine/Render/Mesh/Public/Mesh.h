#pragma once

struct Vertex;
class RHIBuffer;

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

	RHIBuffer* GetVertexBuffer();

	RHIBuffer* GetIndexBuffer();

public:

	static Mesh* GetCube();

	static Mesh* GetPlane();

	static Mesh* GetArrow();

public:

	Vertex *m_pVertices;

	unsigned int m_vertexCount;

	unsigned int *m_pIndices;

	unsigned int m_indexCount;

	glm::vec3 m_boundingBoxMin;

	glm::vec3 m_boundingBoxMax;

private:

	RHIBuffer *m_pVertexBuffer;

	RHIBuffer *m_pIndexBuffer;

};

class MeshLibrary
{
public:

	MeshLibrary();

	~MeshLibrary();

private:

	std::vector<Mesh*> m_pMeshes;

};