#include "pch.h"
#include "Scene/Components/Public/MeshFilter.h"
#include "Render/Public/Vertex.h"
#include "Utils/Public/OBJLoader.h"

std::vector<MeshFilter*> MeshLibrary::m_meshes;

MeshFilter::MeshFilter()
{

}

void MeshFilter::AddVertices(Vertex* head, size_t size)
{
	size_t verts_num = m_vertices.size();
	m_vertices.resize(m_vertices.size() + size);
	for (size_t i = verts_num; i < verts_num + size; ++i)
	{
		m_vertices[i] = head[i - verts_num];
	}
}

void MeshFilter::AddIndices(uint32_t* head, size_t size)
{
	size_t index_num = m_indices.size();
	m_indices.resize(m_indices.size() + size);
	for (size_t i = index_num; i < index_num + size; ++i)
	{
		m_indices[i] = head[i - index_num];
	}
}

std::vector<Vertex>& MeshFilter::GetVertices()
{
	return m_vertices;
}

std::vector<uint32_t>& MeshFilter::GetIndices()
{
	return m_indices;
}

MeshFilter* MeshLibrary::Allocate()
{
	m_meshes.emplace_back(new MeshFilter());
	return m_meshes[m_meshes.size() - 1];
}

MeshFilter* MeshLibrary::Allocate( const char* filePath )
{
	MeshFilter* mesh = OBJLoader::ModelLoad(filePath);
	m_meshes.emplace_back(mesh);
	return mesh;
}