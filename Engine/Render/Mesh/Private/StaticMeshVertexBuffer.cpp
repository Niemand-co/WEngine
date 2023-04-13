#include "pch.h"
#include "Render/Mesh/Public/StaticMeshVertexBuffer.h"
#include "Render/Mesh/Public/StaticMeshVertexData.h"
#include "Render/Mesh/Public/StaticMeshIndexData.h"
#include "Render/Mesh/Public/Vertex.h"

WStaticMeshVertexBuffer::WStaticMeshVertexBuffer()
	: TangentData(new WStaticMeshVertexData<WTangentData>),
	  TexCoordsData(new WStaticMeshVertexData<glm::vec2>)
{
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

	TexCoordsData->ResizeBuffer(InVertices.Size() * 8);
	for (uint32 VertexIndex = 0; VertexIndex < InVertices.Size(); ++VertexIndex)
	{
		glm::vec2* DataPtr = (glm::vec2*)(TexCoordsData->GetData()) + VertexIndex;
		for (uint32 UVChannel = 0; UVChannel < 8; ++UVChannel)
		{
			glm::vec2 *UVPtr = DataPtr + UVChannel;
			*UVPtr = InVertices[VertexIndex].UVs[UVChannel];
		}
	}
}

void WStaticMeshVertexBuffer::InitRHIResource()
{
	TangentBuffer.GetRHI() = GetRenderCommandList()->CreateVertexBuffer(TangentData->GetData(), TangentData->GetStride(), TangentData->GetNum(), EBufferUsageFlags::BF_VertexBuffer | EBufferUsageFlags::BF_TransferDst);
	TexCoordsBuffer.GetRHI() = GetRenderCommandList()->CreateVertexBuffer(TangentData->GetData(), TexCoordsData->GetStride(), TexCoordsData->GetNum(), EBufferUsageFlags::BF_VertexBuffer | EBufferUsageFlags::BF_TransferDst);
}

void WStaticMeshVertexBuffer::ReleaseRHIResource()
{
}

void WStaticMeshVertexBuffer::UpdateRHIResource()
{
}

void WStaticMeshVertexBuffer::BindMeshVertexBuffer(WStaticMeshDataType& Data) const
{
	Data.NormalComponent = WVertexStreamComponent(&TangentBuffer, offsetof(WTangentData, Normal), TangentData->GetStride(), EVertexElementType::VET_Float3);
	Data.TangentComponent = WVertexStreamComponent(&TangentBuffer, offsetof(WTangentData, Tangent), TangentData->GetStride(), EVertexElementType::VET_Float3);

	for (uint32 UVChannel = 0; UVChannel < 8; ++UVChannel)
	{
		Data.TexCoordsComponent[UVChannel] = WVertexStreamComponent(&TexCoordsBuffer, UVChannel * sizeof(glm::vec2), TexCoordsData->GetStride(), EVertexElementType::VET_Float2);
	}
}
