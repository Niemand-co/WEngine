#include "pch.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Render/Descriptor/Public/RHIVertexInputDescriptor.h"

Mesh::Mesh()
{
	m_pVertices = nullptr;
	m_vertexCount = 0;
	m_pIndices = nullptr;
	m_indexCount = 0;
	m_bindingDescription = nullptr;
}

Mesh::~Mesh()
{
}

void Mesh::GenerateVertexInputDescription()
{
	m_bindingDescription = (VertexBindingDescription*)WEngine::Allocator::Get()->Allocate(sizeof(VertexBindingDescription));
	VertexAttributeDescription *attributeDescription = (VertexAttributeDescription*)WEngine::Allocator::Get()->Allocate(3 * sizeof(VertexAttributeDescription));
	m_attributeDescriptions.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		m_attributeDescriptions[i] = attributeDescription + i;
	}

	m_bindingDescription->slot = 0;
	m_bindingDescription->stride = sizeof(Vertex);

	m_attributeDescriptions[0]->slot = 0;
	m_attributeDescriptions[0]->location = 0;
	m_attributeDescriptions[0]->offset = 0;
	m_attributeDescriptions[0]->format = Format::R32G32B32_SFloat;

	m_attributeDescriptions[1]->slot = 0;
	m_attributeDescriptions[1]->location = 1;
	m_attributeDescriptions[1]->offset = sizeof(Vector3);
	m_attributeDescriptions[1]->format = Format::R32G32B32_SFloat;

	m_attributeDescriptions[2]->slot = 0;
	m_attributeDescriptions[2]->location = 2;
	m_attributeDescriptions[2]->offset = sizeof(Vector3) * 2;
	m_attributeDescriptions[2]->format = Format::R32G32_SFloat;
}

RHIVertexInputDescriptor Mesh::GetVertexInputDescriptor()
{
	if (m_bindingDescription == nullptr || m_attributeDescriptions.size() == 0)
	{
		GenerateVertexInputDescription();
	}

	RHIVertexInputDescriptor vertexInputDescriptor = {};
	{
		vertexInputDescriptor.bindingDescription = m_bindingDescription;
		vertexInputDescriptor.attributeDescriptionCount = 3;
		vertexInputDescriptor.pAttributeDescription = m_attributeDescriptions.data();
	}

	return vertexInputDescriptor;
}

Mesh* Mesh::GetCube()
{
	Mesh *mesh = new Mesh();
	Vertex *pVertices = (Vertex*)WEngine::Allocator::Get()->Allocate(8 * sizeof(Vertex));
	{
		pVertices[0] = { { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {} };
		pVertices[1] = { { 1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {} };
		pVertices[2] = { { 1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {} };
		pVertices[3] = { { -1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {} };
		pVertices[4] = { { -1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {} };
		pVertices[5] = { { 1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {} };
		pVertices[6] = { { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {} };
		pVertices[7] = { { -1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {} };
	}
	mesh->m_vertexCount = 8;
	mesh->m_pVertices = pVertices;
	unsigned int *pIndices = (unsigned int*)WEngine::Allocator::Get()->Allocate(36 * sizeof(unsigned int));
	{
		pIndices[0] = 3, pIndices[1] = 2, pIndices[2] = 1, pIndices[3] = 1, pIndices[4] = 0, pIndices[5] = 3,
		pIndices[6] = 0, pIndices[7] = 1, pIndices[8] = 5, pIndices[9] = 5, pIndices[10] = 4, pIndices[11] = 0,
		pIndices[12] = 1, pIndices[13] = 2, pIndices[14] = 6, pIndices[15] = 6, pIndices[16] = 5, pIndices[17] = 1,
		pIndices[18] = 2, pIndices[19] = 3, pIndices[20] = 7, pIndices[21] = 7, pIndices[22] = 6, pIndices[23] = 2,
		pIndices[24] = 3, pIndices[25] = 0, pIndices[26] = 4, pIndices[27] = 4, pIndices[28] = 7, pIndices[29] = 3,
		pIndices[30] = 4, pIndices[31] = 5, pIndices[32] = 6, pIndices[33] = 6, pIndices[34] = 7, pIndices[35] = 4;
	};
	mesh->m_indexCount = 36;
	mesh->m_pIndices = pIndices;
	return mesh;
}
