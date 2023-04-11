#pragma once
#include "Editor/Public/Ray.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Render/Mesh/Public/StaticMeshVertexBuffer.h"
#include "Render/Mesh/Public/StaticMeshIndexBuffer.h"
#include "Render/Mesh/Public/PositionVertexBuffer.h"
#include "Render/Mesh/Public/ColorVertexBuffer.h"

struct VertexComponent;
class RHIBuffer;

struct BoundingBox
{
	glm::vec3 BoxMin;
	glm::vec3 BoxMax;
};

class WStaticMeshVertexBuffers
{
public:

	WStaticMeshVertexBuffers() = default;

	~WStaticMeshVertexBuffers() = default;

	void Init(const WEngine::WArray<VertexComponent>& InVertices);

	void InitResources();

public:

	WStaticMeshVertexBuffer MeshBuffer;

	WPositionVertexBuffer PositionBuffer;

	WColorVertexBuffer ColorBuffer;

};

struct StaticMeshLodResource
{
	uint8 bCastShadow : 1;

	uint8 bForceOpaque : 1;

	WStaticMeshVertexBuffers VertexBuffer;

	WStaticMeshIndexBuffer IndexBuffer;

	uint32 NumIndices;

	uint32 NumVertices;
	
};

struct StaticMeshFactories
{
	WLocalVertexFactory VertexFactory;

	WLocalVertexFactory VertexFactoryWithColor;

	void InitResource(const StaticMeshLodResource& LodResource);
};

struct WStaticMeshRenderData
{
	WEngine::WArray<StaticMeshLodResource> LodResources;

	WEngine::WArray<StaticMeshFactories> Factories;

	WStaticMesh *Owner;

	void Init(const WEngine::WArray<VertexComponent>& InVertices, const WEngine::WArray<uint32>& InIndices);

	void InitResources();
};

class WStaticMesh : public RenderResource
{
public:

	friend class WMeshLibrary;

	friend class WEngine::Ray;

	WStaticMesh(const char* name);

	WStaticMesh(const WEngine::WString& name);

	~WStaticMesh();

	WStaticMeshRenderData* GetRenderData() { return &RenderData; }

	void GenerateBoundingBox();

	const BoundingBox& GetBoundingBox() { return m_boundingBox; }

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

public:

	static WStaticMesh* GetSphere();

	//static StaticMesh* GetCube();

	//static StaticMesh* GetPlane();

	//static StaticMesh* GetArrow();

private:

	WEngine::WString m_name;

	const WEngine::WGuid<WEngine::WString> Id;

	WStaticMeshRenderData RenderData;

	WEngine::WArray<VertexComponent> Vertices;

	WEngine::WArray<uint32> Indices;

	BoundingBox m_boundingBox;

};

size_t MeshHash(WEngine::WString key);

class WMeshLibrary
{
public:

	static WStaticMesh* GetMesh(const WEngine::WString& Name)
	{ 
		return Meshes[Name];
	}

	static bool LoadMesh(const WEngine::WString& Path);

	static bool ProcessNode(const aiNode *Node, const aiScene *ObjectScene, WStaticMesh *Mesh);

	static bool ProcessPrimitive(const aiMesh *Primitive, const aiScene *ObjectScene, WStaticMesh *Mesh);

private:

	static WEngine::WHashMap<WEngine::WString, WStaticMesh*, MeshHash> Meshes;

};