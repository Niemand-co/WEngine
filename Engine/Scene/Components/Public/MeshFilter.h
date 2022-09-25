#pragma once
#ifndef SR_MESH_H
#define SR_MESH_H

#include "Scene/Components/Public/Component.h"

struct Vertex;

class MeshFilter : public Component
{
public:
	MeshFilter();
	~MeshFilter() = default;

	void AddVertices(Vertex* head, size_t size);
	void AddIndices(uint32_t* head, size_t size);

	std::vector<Vertex>& GetVertices();
	std::vector<uint32_t>& GetIndices();

private:
	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
};

class MeshLibrary
{
public:
	static MeshFilter* Allocate();
	static MeshFilter* Allocate( const char* filePath );
private:
	static std::vector<MeshFilter*> m_meshes;
};

#endif