#include "pch.h"
#include "Render/Mesh/Public/StaticMeshVertexBuffer.h"
#include "Render/Mesh/Public/StaticMeshVertexData.h"
#include "Render/Mesh/Public/StaticMeshIndexData.h"
#include "Render/Mesh/Public/Vertex.h"

WStaticMeshVertexBuffer::WStaticMeshVertexBuffer()
{
	TangentData = new WStaticMeshVertexData<WTangentData>();
	TexCoordsData = new WStaticMeshVertexData<glm::vec2>();
}

WStaticMeshVertexBuffer::~WStaticMeshVertexBuffer()
{
}

void WStaticMeshVertexBuffer::Init(const WEngine::WArray<VertexComponent>& InVertices)
{
	TangentData->ResizeBuffer(InVertices.Size());
	for (uint32 VertexIndex = 0; VertexIndex < InVertices.Size(); ++VertexIndex)
	{
		WTangentData *DataPtr = (WTangentData*)(TangentData->GetData()) + VertexIndex;
		*DataPtr = WTangentData(InVertices[VertexIndex].Normal, InVertices[VertexIndex].Tangent);
	}

	TangentData->ResizeBuffer(InVertices.Size());
	for (uint32 VertexIndex = 0; VertexIndex < InVertices.Size(); ++VertexIndex)
	{
		glm::vec2* DataPtr = (glm::vec2*)(TangentData->GetData()) + VertexIndex;
		*DataPtr = InVertices[VertexIndex].UVs[0];
	}
}

void WStaticMeshVertexBuffer::InitRHIResource()
{
}

void WStaticMeshVertexBuffer::ReleaseRHIResource()
{
}

void WStaticMeshVertexBuffer::UpdateRHIResource()
{
}
