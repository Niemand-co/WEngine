#pragma once

struct Vertex;
class RHIBuffer;

class Mesh : public WEngine::NamingSystem, public RenderResource
{
public:

	Mesh(const char *name);

	Mesh(const WEngine::WString& name);

	~Mesh();

	RHIBuffer* GetVertexBuffer();

	RHIBuffer* GetIndexBuffer();

public:

	static Mesh* GetCube();

	static Mesh* GetPlane();

	static Mesh* GetArrow();

public:

	const WEngine::WGuid m_id;

	Vertex *m_pVertices;

	unsigned int m_vertexCount;

	unsigned int *m_pIndices;

	unsigned int m_indexCount;

	glm::vec3 m_boundingBoxMin;

	glm::vec3 m_boundingBoxMax;

private:

	WEngine::WString m_name;

	RHIBuffer *m_pVertexBuffer;

	RHIBuffer *m_pIndexBuffer;

};

class MeshLibrary
{
public:

	MeshLibrary();

	~MeshLibrary();

	void AddMesh(const WEngine::WGuid& id, Mesh *pMesh);

	Mesh* GetMesh(const WEngine::WGuid& id);

private:

	WEngine::WArray<WEngine::WPair<WEngine::WGuid, Mesh*>> m_meshes;

};