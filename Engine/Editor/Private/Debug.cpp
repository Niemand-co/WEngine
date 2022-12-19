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

	WArray<Vertex> Debug::g_lineVertices = WArray<Vertex>();

	WArray<unsigned int> Debug::g_lineIndices = WArray<unsigned int>();

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
		g_lineVertices.Push(Vertex(a, color));
		g_lineVertices.Push(Vertex(b, color));

		g_lineIndices.Push(g_lineIndices.Size());
		g_lineIndices.Push(g_lineIndices.Size());
	}

	void Debug::Update()
	{
		g_pLineVertexBuffer->LoadData(g_lineVertices.GetData(), g_lineVertices.Size() * sizeof(Vertex), 0);
		g_pLineIndexBuffer->LoadData(g_lineIndices.GetData(), g_lineIndices.Size() * sizeof(unsigned int), 0);

		g_lineVertices.Clear();
		g_lineIndices.Clear();
	}

}