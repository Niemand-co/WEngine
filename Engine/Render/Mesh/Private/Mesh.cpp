#include "pch.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Render/Descriptor/Public/RHIVertexInputDescriptor.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIBuffer.h"

Mesh::Mesh()
{
	m_pVertices = nullptr;
	m_vertexCount = 0;
	m_pIndices = nullptr;
	m_indexCount = 0;
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
}

Mesh::~Mesh()
{
}

RHIBuffer* Mesh::GetVertexBuffer()
{
	if(m_pVertexBuffer != nullptr)
		return m_pVertexBuffer;
	
	RHIBufferDescriptor descriptor = {};
	{
		descriptor.dataSize = m_vertexCount * sizeof(Vertex);
		descriptor.isDynamic = false;
		descriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pVertexBuffer = RHIContext::GetContext()->CreateVertexBuffer(&descriptor);
	m_pVertexBuffer->LoadData(m_pVertices, m_vertexCount * sizeof(Vertex));

	return m_pVertexBuffer;
}

RHIBuffer* Mesh::GetIndexBuffer()
{
	if(m_pIndexBuffer != nullptr)
		return m_pIndexBuffer;

	RHIBufferDescriptor descriptor = {};
	{
		descriptor.dataSize = m_indexCount * sizeof(unsigned int);
		descriptor.isDynamic = false;
		descriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pIndexBuffer = RHIContext::GetContext()->CreateIndexBuffer(&descriptor);
	m_pIndexBuffer->LoadData(m_pIndices, m_indexCount * sizeof(unsigned int));

	return m_pIndexBuffer;
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

	mesh->m_boundingBoxMin = glm::vec3(-1.0f, -1.0f, -1.0f);
	mesh->m_boundingBoxMax = glm::vec3(1.0f, 1.0f, 1.0f);

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

	mesh->m_boundingBoxMin = glm::vec3(-1.0f, 0.0f, -1.0f);
	mesh->m_boundingBoxMax = glm::vec3(1.0f, 0.0f, 1.0f);

	return mesh;
}

MeshLibrary::MeshLibrary()
{
}

MeshLibrary::~MeshLibrary()
{
}
