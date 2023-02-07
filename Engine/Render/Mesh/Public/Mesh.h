#pragma once

struct VertexComponent;
class RHIBuffer;

struct BoundingBox
{
	glm::vec3 BoxMin;
	glm::vec3 BoxMax;
};

class StaticMesh : public WEngine::NamingSystem, public RenderResource
{
public:

	StaticMesh(const char *name);

	StaticMesh(const WEngine::WString& name);

	~StaticMesh();

	RHIBuffer* GetVertexBuffer();

	RHIBuffer* GetIndexBuffer();

	void GenerateBoundingBox();

	const BoundingBox& GetBoundingBox() { return m_boundingBox; }

public:

	//static StaticMesh* GetCube();

	//static StaticMesh* GetPlane();

	//static StaticMesh* GetArrow();

private:

	WEngine::WString m_name;

	RHIBuffer *m_pVertexBuffer;

	RHIBuffer *m_pIndexBuffer;

public:

	const WEngine::WGuid<WEngine::WString> m_id;

	WEngine::WArray<VertexComponent> m_vertices;

	unsigned int m_vertexCount;

	unsigned int *m_pIndices;

	unsigned int m_indexCount;

	BoundingBox m_boundingBox;

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