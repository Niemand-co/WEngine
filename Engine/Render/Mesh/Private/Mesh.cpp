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
	VertexAttributeDescription *attributeDescription = (VertexAttributeDescription*)WEngine::Allocator::Get()->Allocate(4 * sizeof(VertexAttributeDescription));
	m_attributeDescriptions.resize(4);
	for (int i = 0; i < 4; ++i)
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
	m_attributeDescriptions[2]->format = Format::R32G32B32_SFloat;

	m_attributeDescriptions[3]->slot = 0;
	m_attributeDescriptions[3]->location = 3;
	m_attributeDescriptions[3]->offset = sizeof(Vector3) * 3;
	m_attributeDescriptions[3]->format = Format::R32G32_SFloat;
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
		vertexInputDescriptor.attributeDescriptionCount = m_attributeDescriptions.size();
		vertexInputDescriptor.pAttributeDescription = m_attributeDescriptions.data();
	}

	return vertexInputDescriptor;
}

Mesh* Mesh::GetCube()
{
	Mesh *mesh = new Mesh();
	Vertex *pVertices = (Vertex*)WEngine::Allocator::Get()->Allocate(24 * sizeof(Vertex));
	{
		pVertices[0] = { { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[1] = { { 1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f} };
		pVertices[2] = { { 1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f} };
		pVertices[3] = { { -1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f} };

		pVertices[4] = { { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f} };
		pVertices[5] = { { 1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f} };
		pVertices[6] = { { 1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f} };
		pVertices[7] = { { -1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f} };

		pVertices[8] = { { 1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[9] = { { 1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} };
		pVertices[10] = { { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f} };
		pVertices[11] = { { 1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} };

		pVertices[12] = { { 1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} };
		pVertices[13] = { { -1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} };
		pVertices[14] = { { -1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} };
		pVertices[15] = { { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} };

		pVertices[16] = { { -1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[17] = { { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} };
		pVertices[18] = { { -1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f} };
		pVertices[19] = { { -1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} };

		pVertices[20] = { { -1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[21] = { { 1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} };
		pVertices[22] = { { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} };
		pVertices[23] = { { -1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} };
	}
	mesh->m_vertexCount = 24;
	mesh->m_pVertices = pVertices;
	unsigned int *pIndices = (unsigned int*)WEngine::Allocator::Get()->Allocate(36 * sizeof(unsigned int));
	{
		pIndices[0] = 3, pIndices[1] = 2, pIndices[2] = 1, pIndices[3] = 1, pIndices[4] = 0, pIndices[5] = 3,
		pIndices[6] = 4, pIndices[7] = 5, pIndices[8] = 6, pIndices[9] = 6, pIndices[10] = 7, pIndices[11] = 4,
		pIndices[12] = 8, pIndices[13] = 9, pIndices[14] = 10, pIndices[15] = 10, pIndices[16] = 11, pIndices[17] = 8,
		pIndices[18] = 12, pIndices[19] = 13, pIndices[20] = 14, pIndices[21] = 14, pIndices[22] = 15, pIndices[23] = 12,
		pIndices[24] = 16, pIndices[25] = 17, pIndices[26] = 18, pIndices[27] = 18, pIndices[28] = 19, pIndices[29] = 16,
		pIndices[30] = 20, pIndices[31] = 21, pIndices[32] = 22, pIndices[33] = 22, pIndices[34] = 23, pIndices[35] = 20;
	};
	mesh->m_indexCount = 36;
	mesh->m_pIndices = pIndices;
	return mesh;
}

Mesh* Mesh::GetPlane()
{
	Mesh *mesh = new Mesh();
	Vertex* pVertices = (Vertex*)WEngine::Allocator::Get()->Allocate(4 * sizeof(Vertex));
	{
		pVertices[0] = { { -1.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[1] = { { 1.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} };
		pVertices[2] = { { 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} };
		pVertices[3] = { { -1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} };
	}
	mesh->m_vertexCount = 4;
	mesh->m_pVertices = pVertices;
	unsigned int* pIndices = (unsigned int*)WEngine::Allocator::Get()->Allocate(6 * sizeof(unsigned int));
	{
		pIndices[0] = 0, pIndices[1] = 1, pIndices[2] = 2, pIndices[3] = 2, pIndices[4] = 3, pIndices[5] = 0;
	}
	mesh->m_indexCount = 6;
	mesh->m_pIndices = pIndices;
	return mesh;
}

MeshLibrary::MeshLibrary()
{
}

MeshLibrary::~MeshLibrary()
{
}
