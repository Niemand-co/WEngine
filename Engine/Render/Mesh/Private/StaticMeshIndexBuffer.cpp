#include "pch.h"
#include "Render/Mesh/Public/StaticMeshIndexBuffer.h"
#include "Render/Mesh/Public/StaticMeshIndexData.h"

WStaticMeshIndexBuffer::WStaticMeshIndexBuffer()
	: IndexData(new WStaticMeshIndexData)
{
}

WStaticMeshIndexBuffer::~WStaticMeshIndexBuffer()
{
}

void WStaticMeshIndexBuffer::Init(const WEngine::WArray<uint32>& InIndices)
{
	uint32 NumIndices = InIndices.Size();
	IndexData->ResizeBuffer(NumIndices);
	memcpy(IndexData->GetData(), InIndices.GetData(), NumIndices * sizeof(uint32));
}

void WStaticMeshIndexBuffer::InitRHIResource()
{
	Buffer = GetRenderCommandList()->CreateIndexBuffer(IndexData->GetData(), IndexData->GetNum(), EBufferUsageFlags::BF_IndexBuffer);
}

void WStaticMeshIndexBuffer::UpdateRHIResource()
{
}
