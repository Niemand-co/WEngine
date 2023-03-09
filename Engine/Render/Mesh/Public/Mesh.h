#pragma once
#include "Editor/Public/Ray.h"

struct VertexComponent;
class RHIBuffer;

struct BoundingBox
{
	glm::vec3 BoxMin;
	glm::vec3 BoxMax;
};

class WStaticMeshRenderData
{

};

class WStaticMesh
{
public:

	friend class WMeshLibrary;

	friend class WEngine::Ray;

	WStaticMesh(const char* name);

	WStaticMesh(const WEngine::WString& name);

	~WStaticMesh();

	void GenerateBoundingBox();

	const BoundingBox& GetBoundingBox() { return m_boundingBox; }

	void* operator new(size_t size)
	{
		return NormalAllocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		NormalAllocator::Get()->Deallocate(pData);
	}

public:

	static WStaticMesh* GetSphere();

	//static StaticMesh* GetCube();

	//static StaticMesh* GetPlane();

	//static StaticMesh* GetArrow();

private:

	WEngine::WString m_name;

	const WEngine::WGuid<WEngine::WString> m_id;

	WEngine::WArray<VertexComponent> m_vertices;

	WEngine::WArray<uint32> m_indices;

	BoundingBox m_boundingBox;

};

size_t MeshHash(WEngine::WGuid<WEngine::WString> key);

class WMeshLibrary
{
public:

	static WStaticMesh* GetMesh(const WEngine::WString& Name)
	{ 
		return Meshes[WEngine::WGuid(Name)];
	}

	static bool LoadMesh(const WEngine::WString& Path);

	static bool ProcessNode(const aiNode *Node, const aiScene *ObjectScene, WStaticMesh *Mesh);

	static bool ProcessPrimitive(const aiMesh *Primitive, const aiScene *ObjectScene, WStaticMesh *Mesh);

private:

	static WEngine::WHashMap<WEngine::WGuid<WEngine::WString>, WStaticMesh*, MeshHash> Meshes;

};