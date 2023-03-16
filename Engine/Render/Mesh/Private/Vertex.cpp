#include "pch.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Render/Descriptor/Public/RHIVertexInputDescriptor.h"
#include "Render/Public/Buffer.h"

WVertexFactory::WVertexFactory()
{
}

WVertexFactory::~WVertexFactory()
{
}

void WVertexFactory::InitRHIResource()
{
}

void WVertexFactory::ReleaseRHIResource()
{
}

void WVertexFactory::UpdateRHIResource()
{
}

void WLocalVertexFactory::GetPSOVertexInputElements(EVertexInputType Type, WEngine::WArray<VertexInputElement>& Elements)
{
	Elements.Push(VertexInputElement(0, 0, VertexElementType::VET_Float3, 0, 12, false));

	switch (Type)
	{
	case EVertexInputType::Default:
		Elements.Push(VertexInputElement(1, 0, VertexElementType::VET_UInt, 13, 0, true));
		break;
	case EVertexInputType::PositionOnly:
		Elements.Push(VertexInputElement(1, 0, VertexElementType::VET_UInt, 1, 0, true));
		break;
	case EVertexInputType::PositionAndNormal:
		Elements.Push(VertexInputElement(1, 4, VertexElementType::VET_PackedNormal, 2, 0, false));
		Elements.Push(VertexInputElement(2, 0, VertexElementType::VET_UInt, 1, 0, true));
		break;
	default:
		break;
	}
}

RHIVertexInputDescriptor WLocalVertexFactory::GetDeclaration(const WEngine::WArray<VertexInputElement>& Elements)
{
	RHIVertexInputDescriptor VertexInputDescriptor = {};
	uint32 NumElements = Elements.Size();
	VertexInputDescriptor.attributeDescriptionCount = NumElements;
	for(uint32 ElementIndex = 0; ElementIndex < NumElements; ++ElementIndex)
	{
		VertexInputDescriptor.Generate
	}
}
