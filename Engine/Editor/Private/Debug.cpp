#include "pch.h"
#include "Editor/Public/Debug.h"
#include "Render/Mesh/Public/Vertex.h"
#include "RHI/Public/RHIBuffer.h"
#include "RHI/Public/RHIContext.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"

namespace WEngine
{

	RHIBuffer* Debug::g_pLineVertexBuffer = nullptr;

	RHIBuffer* Debug::g_pLineIndexBuffer = nullptr;

	std::vector<Vertex> Debug::g_lineVertices = std::vector<Vertex>();

	std::vector<unsigned int> Debug::g_lineIndices = std::vector<unsigned int>();

	void Debug::Init()
	{
		RHIBufferDescriptor vertexDescriptor = {};
		{
			vertexDescriptor.count = 100;
			vertexDescriptor.dataSize = sizeof(Vertex);
			vertexDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
		}
		g_pLineVertexBuffer = RHIContext::GetContext()->CreateVertexBuffer(&vertexDescriptor);

		RHIBufferDescriptor indexDescriptor = {};
		{
			indexDescriptor.count = 100;
			indexDescriptor.dataSize = sizeof(unsigned int);
			indexDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
		}
		g_pLineIndexBuffer = RHIContext::GetContext()->CreateIndexBuffer(&indexDescriptor);
	}

	void Debug::DrawLine(glm::vec3 a, glm::vec3 b, glm::vec4 color)
	{
		g_lineVertices.emplace_back(Vertex(a, color));
		g_lineVertices.emplace_back(Vertex(b, color));

		g_lineIndices.emplace_back(g_lineIndices.size());
		g_lineIndices.emplace_back(g_lineIndices.size());
	}

	void Debug::Update()
	{
		g_pLineVertexBuffer->LoadData(g_lineVertices.data(), g_lineVertices.size() * sizeof(Vertex), 0);
		g_pLineIndexBuffer->LoadData(g_lineIndices.data(), g_lineIndices.size() * sizeof(unsigned int), 0);

		g_lineVertices.clear();
		g_lineIndices.clear();
	}

}