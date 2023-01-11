#pragma once

struct Vertex;
class RHIBuffer;

class StaticMesh : public WEngine::NamingSystem, public RenderResource
{
public:

	StaticMesh(const char *name);

	StaticMesh(const WEngine::WString& name);

	~StaticMesh();

	RHIBuffer* GetVertexBuffer();

	RHIBuffer* GetIndexBuffer();

public:

	static StaticMesh* GetCube();

	static StaticMesh* GetPlane();

	static StaticMesh* GetArrow();

private:

	WEngine::WString m_name;

	RHIBuffer *m_pVertexBuffer;

	RHIBuffer *m_pIndexBuffer;

public:

	const WEngine::WGuid<WEngine::WString> m_id;

	Vertex *m_pVertices;

	unsigned int m_vertexCount;

	unsigned int *m_pIndices;

	unsigned int m_indexCount;

	glm::vec3 m_boundingBoxMin;

	glm::vec3 m_boundingBoxMax;

};

class MeshLibrary
{
public:

	MeshLibrary();

	~MeshLibrary();

	void AddMesh(const WEngine::WGuid<WEngine::WString>& id, StaticMesh*pMesh);

	StaticMesh* GetMesh(const WEngine::WGuid<WEngine::WString>& id);

private:

	WEngine::WArray<WEngine::WPair<WEngine::WGuid<WEngine::WString>, StaticMesh*>> m_meshes;

};