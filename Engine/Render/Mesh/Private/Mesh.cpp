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
	float sqrt2frag2 = std::sin(PI / 4.0f) * 0.02f;
	float sqrt2 = 2.0f * sqrt2frag2;
	Mesh *mesh = new Mesh();
	Vertex *pVertices = (Vertex*)WEngine::Allocator::Get()->Allocate(27 * sizeof(Vertex));
	{
		pVertices[0] = { { 0.0f, 0.0f, 0.02f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[1] = { { sqrt2frag2, 0.0f, sqrt2frag2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[2] = { { 0.02f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[3] = { { sqrt2frag2, 0.0f, -sqrt2frag2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[4] = { { 0.0f, 0.0f, -0.02f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[5] = { { -sqrt2frag2, 0.0f, -sqrt2frag2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[6] = { { -0.02f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[7] = { { -sqrt2frag2, 0.0f, sqrt2frag2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };

		pVertices[8] = { { 0.0f, 0.8f, 0.02f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[9] = { { sqrt2frag2, 0.8f, sqrt2frag2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[10] = { { 0.02f, 0.8f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[11] = { { sqrt2frag2, 0.8f, -sqrt2frag2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[12] = { { 0.0f, 0.8f, -0.02f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[13] = { { -sqrt2frag2, 0.8f, -sqrt2frag2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[14] = { { -0.02f, 0.8f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[15] = { { -sqrt2frag2, 0.8f, sqrt2frag2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };

		pVertices[16] = { { 0.0f, 0.8f, 0.04f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[17] = { { sqrt2, 0.8f, sqrt2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[18] = { { 0.04f, 0.8f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[19] = { { sqrt2, 0.8f, -sqrt2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[20] = { { 0.0f, 0.8f, -0.04f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[21] = { { -sqrt2, 0.8f, -sqrt2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[22] = { { -0.04f, 0.8f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[23] = { { -sqrt2, 0.8f, sqrt2 }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };

		pVertices[24] = { { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[25] = { { 0.0f, 0.8f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		pVertices[26] = { { 0.0f, 1.1f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
	}
	mesh->m_vertexCount = 27;
	mesh->m_pVertices = pVertices;
	unsigned int *pIndices = (unsigned int*)WEngine::Allocator::Get()->Allocate(96 * sizeof(unsigned int));
	{
		pIndices[0] = 0, pIndices[1] = 1, pIndices[2] = 24, pIndices[3] = 1, pIndices[4] = 2, pIndices[5] = 24,
		pIndices[6] = 2, pIndices[7] = 3, pIndices[8] = 24, pIndices[9] = 3, pIndices[10] = 4, pIndices[11] = 24,
		pIndices[12] = 4, pIndices[13] = 5, pIndices[14] = 24, pIndices[15] = 5, pIndices[16] = 6, pIndices[17] = 24,
		pIndices[18] = 6, pIndices[19] = 7, pIndices[20] = 24, pIndices[21] = 7, pIndices[22] = 0, pIndices[23] = 24,

		pIndices[24] = 0, pIndices[25] = 1, pIndices[26] = 9, pIndices[27] = 9, pIndices[28] = 8, pIndices[29] = 0, 
		pIndices[30] = 1, pIndices[31] = 2, pIndices[32] = 10, pIndices[33] = 10, pIndices[34] = 9, pIndices[35] = 1,
		pIndices[36] = 2, pIndices[37] = 3, pIndices[38] = 11, pIndices[39] = 11, pIndices[40] = 10, pIndices[41] = 2,
		pIndices[42] = 3, pIndices[43] = 4, pIndices[44] = 12, pIndices[45] = 12, pIndices[46] = 11, pIndices[47] = 3,
		pIndices[48] = 4, pIndices[49] = 5, pIndices[50] = 13, pIndices[51] = 13, pIndices[52] = 12, pIndices[53] = 4,
		pIndices[54] = 5, pIndices[55] = 6, pIndices[56] = 14, pIndices[57] = 14, pIndices[58] = 13, pIndices[59] = 5,
		pIndices[60] = 6, pIndices[61] = 7, pIndices[62] = 15, pIndices[63] = 15, pIndices[64] = 14, pIndices[65] = 6,
		pIndices[66] = 7, pIndices[67] = 0, pIndices[68] = 8, pIndices[69] = 8, pIndices[70] = 15, pIndices[71] = 7,

		pIndices[72] = 8, pIndices[73] = 9, pIndices[74] = 25, pIndices[75] = 9, pIndices[76] = 10, pIndices[77] = 25,
		pIndices[78] = 10, pIndices[79] = 11, pIndices[80] = 25, pIndices[81] = 11, pIndices[82] = 12, pIndices[83] = 25,
		pIndices[84] = 12, pIndices[85] = 13, pIndices[86] = 25, pIndices[87] = 13, pIndices[88] = 14, pIndices[89] = 25,
		pIndices[90] = 14, pIndices[91] = 15, pIndices[92] = 25, pIndices[93] = 15, pIndices[94] = 8, pIndices[95] = 25,

		pIndices[96] = 16, pIndices[97] = 17, pIndices[98] = 26, pIndices[99] = 17, pIndices[100] = 18, pIndices[101] = 26,
		pIndices[102] = 18, pIndices[103] = 19, pIndices[104] = 26, pIndices[105] = 19, pIndices[106] = 20, pIndices[107] = 26,
		pIndices[108] = 20, pIndices[109] = 21, pIndices[110] = 26, pIndices[111] = 21, pIndices[112] = 22, pIndices[113] = 26,
		pIndices[114] = 22, pIndices[115] = 23, pIndices[116] = 26, pIndices[117] = 23, pIndices[118] = 16, pIndices[119] = 26;
	}
	mesh->m_indexCount = 120;
	mesh->m_pIndices = pIndices;

	mesh->m_boundingBoxMin = glm::vec3(-sqrt2, 0.0f, -sqrt2);
	mesh->m_boundingBoxMax = glm::vec3(sqrt2, 0.8f, sqrt2);

	return mesh;
}

MeshLibrary::MeshLibrary()
{
}

MeshLibrary::~MeshLibrary()
{
}
