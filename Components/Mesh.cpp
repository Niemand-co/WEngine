#include "Mesh.h"

Mesh::Mesh()
{

}

void Mesh::AddVertices(Vertex* head, size_t size)
{
	size_t verts_num = m_vertices.size();
	m_vertices.resize(m_vertices.size() + size);
	for (size_t i = verts_num; i < verts_num + size; ++i)
	{
		m_vertices[i] = head[i - verts_num];
	}
}

void Mesh::AddIndices(uint32_t* head, size_t size)
{
	size_t index_num = m_indices.size();
	m_indices.resize(m_indices.size() + size);
	for (size_t i = index_num; i < index_num + size; ++i)
	{
		m_indices[i] = head[i - index_num];
	}
}

std::vector<Vertex>& Mesh::GetVertices()
{
	return m_vertices;
}

std::vector<uint32_t>& Mesh::GetIndices()
{
	return m_indices;
}