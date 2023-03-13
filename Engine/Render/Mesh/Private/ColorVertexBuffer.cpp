#include "pch.h"
#include "Render/Mesh/Public/ColorVertexBuffer.h"
#include "Render/Mesh/Public/StaticMeshColorData.h"
#include "Render/Mesh/Public/Vertex.h"

WColorVertexBuffer::WColorVertexBuffer()
	: ColorData(new WStaticMeshColorData)
{
}

WColorVertexBuffer::~WColorVertexBuffer()
{
}

void WColorVertexBuffer::Init(const WEngine::WArray<struct VertexComponent>& InVertices)
{
	uint32 NumVertices = InVertices.Size();
	ColorData->ResizeBuffer(NumVertices);
	for (uint32 VertexIndex = 0; VertexIndex < NumVertices; ++VertexIndex)
	{
		glm::vec3 *DataPtr = (glm::vec3*)ColorData->GetData() + VertexIndex;
		*DataPtr = InVertices[VertexIndex].Color;
	}
}

void WColorVertexBuffer::InitRHIResource()
{
	Buffer = GetRenderCommandList()->CreateVertexBuffer(ColorData->GetStride(), ColorData->GetNum(), EBufferUsageFlags::BF_VertexBuffer);
}

void WColorVertexBuffer::UpdateRHIResource()
{
}

void WColorVertexBuffer::BindColorVertexBuffer(WStaticMeshDataType& Data) const
{
	Data.ColorComponent = WVertexStreamComponent(this, 0, ColorData->GetStride(), VertexElementType::VET_Float3);
}
