#include "pch.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Render/Descriptor/Public/RHIVertexInputDescriptor.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIBuffer.h"

#define PI 3.1415926535

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

Mesh* Mesh::GetArrow()
{
	Mesh *mesh = new Mesh();
	Vertex *pVertices = (Vertex*)WEngine::Allocator::Get()->Allocate(27 * sizeof(Vertex));
	{
		float sqrt2frag2 = std::sin(PI / 4.0f) * 0.1f;
		float sqrt2 = 2.0f * sqrt2frag2;
		pVertices[0] = { { 0.0f, 0.0f, 0.1f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[1] = { { sqrt2frag2, 0.0f, sqrt2frag2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[2] = { { 0.1f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[3] = { { sqrt2frag2, 0.0f, -sqrt2frag2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[4] = { { 0.0f, 0.0f, -0.1f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[5] = { { -sqrt2frag2, 0.0f, -sqrt2frag2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[6] = { { -0.1f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[7] = { { -sqrt2frag2, 0.0f, sqrt2frag2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };

		pVertices[8] = { { 0.0f, 0.8f, 0.1f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[9] = { { sqrt2frag2, 0.8f, sqrt2frag2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[10] = { { 0.1f, 0.8f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[11] = { { sqrt2frag2, 0.8f, -sqrt2frag2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[12] = { { 0.0f, 0.8f, -0.1f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[13] = { { -sqrt2frag2, 0.8f, -sqrt2frag2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[14] = { { -0.1f, 0.8f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[15] = { { -sqrt2frag2, 0.8f, sqrt2frag2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };

		pVertices[16] = { { 0.0f, 0.8f, 0.1f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[17] = { { sqrt2, 0.8f, sqrt2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[18] = { { 0.1f, 0.8f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[19] = { { sqrt2, 0.8f, -sqrt2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[20] = { { 0.0f, 0.8f, -0.1f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[21] = { { -sqrt2, 0.8f, -sqrt2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[22] = { { -0.1f, 0.8f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[23] = { { -sqrt2, 0.8f, sqrt2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };

		pVertices[24] = { { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[25] = { { 0.0f, 0.8f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[26] = { { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
	}
	mesh->m_vertexCount = 27;
	mesh->m_pVertices = pVertices;
	unsigned int *pIndices = (unsigned int*)WEngine::Allocator::Get()->Allocate(96 * sizeof(unsigned int));
	{
		pIndices[0] = 0, pIndices[1] = 1, pIndices[2] = 24, pIndices[3] = 1, pIndices[4] = 2, pIndices[5] = 24,
		pIndices[6] = 2, pIndices[7] = 3, pIndices[8] = 24, pIndices[9] = 3, pIndices[10] = 4, pIndices[11] = 24,
		pIndices[12] = 4, pIndices[13] = 5, pIndices[14] = 24, pIndices[15] = 5, pIndices[16] = 6, pIndices[17] = 24,
		pIndices[18] = 6, pIndices[19] = 7, pIndices[20] = 24, pIndices[18] = 7, pIndices[19] = 0, pIndices[20] = 24,

		pIndices[21] = 0, pIndices[22] = 1, pIndices[23] = 9, pIndices[24] = 9, pIndices[25] = 8, pIndices[26] = 0, 
		pIndices[27] = 1, pIndices[28] = 2, pIndices[29] = 10, pIndices[30] = 10, pIndices[31] = 9, pIndices[32] = 1,
		pIndices[33] = 2, pIndices[34] = 3, pIndices[35] = 11, pIndices[36] = 11, pIndices[37] = 10, pIndices[38] = 2,
		pIndices[39] = 3, pIndices[40] = 4, pIndices[41] = 12, pIndices[42] = 12, pIndices[43] = 11, pIndices[44] = 3,
		pIndices[45] = 4, pIndices[46] = 5, pIndices[47] = 13, pIndices[48] = 13, pIndices[49] = 12, pIndices[50] = 4,
		pIndices[51] = 5, pIndices[52] = 6, pIndices[53] = 14, pIndices[54] = 14, pIndices[55] = 13, pIndices[56] = 5,
		pIndices[57] = 6, pIndices[58] = 7, pIndices[59] = 15, pIndices[60] = 15, pIndices[61] = 14, pIndices[62] = 6,
		pIndices[63] = 7, pIndices[64] = 0, pIndices[65] = 8, pIndices[66] = 8, pIndices[67] = 15, pIndices[68] = 7,

		pIndices[0] = 8, pIndices[1] = 9, pIndices[2] = 25, pIndices[3] = 9, pIndices[4] = 10, pIndices[5] = 25,
		pIndices[6] = 10, pIndices[7] = 11, pIndices[8] = 25, pIndices[9] = 11, pIndices[10] = 12, pIndices[11] = 25,
		pIndices[12] = 12, pIndices[13] = 13, pIndices[14] = 25, pIndices[15] = 13, pIndices[16] = 14, pIndices[17] = 25,
		pIndices[18] = 14, pIndices[19] = 15, pIndices[20] = 25, pIndices[18] = 15, pIndices[19] = 8, pIndices[20] = 25,

		pIndices[0] = 16, pIndices[1] = 17, pIndices[2] = 26, pIndices[3] = 17, pIndices[4] = 18, pIndices[5] = 26,
		pIndices[6] = 18, pIndices[7] = 19, pIndices[8] = 26, pIndices[9] = 19, pIndices[10] = 20, pIndices[11] = 26,
		pIndices[12] = 20, pIndices[13] = 21, pIndices[14] = 26, pIndices[15] = 21, pIndices[16] = 22, pIndices[17] = 26,
		pIndices[18] = 22, pIndices[19] = 23, pIndices[20] = 26, pIndices[18] = 23, pIndices[19] = 16, pIndices[20] = 26;
	}
}

MeshLibrary::MeshLibrary()
{
}

MeshLibrary::~MeshLibrary()
{
}
