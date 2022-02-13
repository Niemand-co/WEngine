#pragma once
#ifndef SR_MESH_H
#define SR_MESH_H

#include "Component.h"
#include "Render/Vertex.h"
#include <vector>

class Mesh : public Component
{
public:
	Mesh();
	~Mesh() = default;

	void AddVertices(Vertex* head, size_t size);
	void AddIndices(uint32_t* head, size_t size);

	std::vector<Vertex>& GetVertices();
	std::vector<uint32_t>& GetIndices();

private:
	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
};

#endif