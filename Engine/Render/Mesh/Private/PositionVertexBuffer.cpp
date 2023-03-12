#include "pch.h"
#include "Render/Mesh/Public/PositionVertexBuffer.h"
#include "Render/Mesh/Public/StaticMeshPositionData.h"
#include "Render/Mesh/Public/Vertex.h"

WPositionVertexBuffer::WPositionVertexBuffer()
	: PositionData(new WStaticMeshPositionData)
{
}

WPositionVertexBuffer::~WPositionVertexBuffer()
{
}

void WPositionVertexBuffer::Init(const WEngine::WArray<VertexComponent>& InVertices)
{
	uint32 NumVertices = InVertices.Size();
	PositionData->ResizeBuffer(NumVertices);
	for (uint32 VertexIndex = 0; VertexIndex < NumVertices; ++VertexIndex)
	{
		glm::vec3 *DataPtr = (glm::vec3*)PositionData->GetData() + VertexIndex;
		*DataPtr = InVertices[VertexIndex].Position;
	}
}

void WPositionVertexBuffer::InitRHIResource()
{
	Buffer = GetRenderCommandList()->CreateVertexBuffer(PositionData->GetStride(), PositionData->GetNum(), EBufferUsageFlags::BF_VertexBuffer);
}

void WPositionVertexBuffer::UpdateRHIResource()
{
}
