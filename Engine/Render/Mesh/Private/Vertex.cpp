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
	{
	}
}

void WVertexFactory::ReleaseRHIResource()
{
}

void WVertexFactory::UpdateRHIResource()
{
}

void WLocalVertexFactory::InitRHIResource()
{
	{
		WEngine::WArray<VertexInputElement> Elements;
		Elements.Push(VertexInputElement(0, 0, EVertexElementType::VET_Float3, 0, 12, false));
		PositionDeclaration = GetRenderCommandList()->GetOrCreateVertexInputState(Elements);
	}

	{
		WEngine::WArray<VertexInputElement> Elements;
		Elements.Push(VertexInputElement(0, 0, EVertexElementType::VET_Float3, 0, 12, false));
		Elements.Push(VertexInputElement(1, 12, EVertexElementType::VET_Float3, 0, 12, false));
		PositionAndNormalDeclaration = GetRenderCommandList()->GetOrCreateVertexInputState(Elements);
	}
}

void WLocalVertexFactory::ReleaseRHIResource()
{
}

void WLocalVertexFactory::UpdateRHIResource()
{
}

void WLocalVertexFactory::GetPSOVertexInputElements(EVertexInputType Type, WEngine::WArray<VertexInputElement>& Elements)
{
	Elements.Push(VertexInputElement(0, 0, EVertexElementType::VET_Float3, 0, 12, false));

	switch (Type)
	{
	case EVertexInputType::Default:
		Elements.Push(VertexInputElement(1, 0, EVertexElementType::VET_UInt, 13, 0, true));
		break;
	case EVertexInputType::PositionOnly:
		Elements.Push(VertexInputElement(1, 0, EVertexElementType::VET_UInt, 1, 0, true));
		break;
	case EVertexInputType::PositionAndNormal:
		Elements.Push(VertexInputElement(1, 4, EVertexElementType::VET_PackedNormal, 2, 0, false));
		Elements.Push(VertexInputElement(2, 0, EVertexElementType::VET_UInt, 1, 0, true));
		break;
	default:
		break;
	}
}

WVertexInputStateRHIRef WLocalVertexFactory::GetVertexInput(EVertexInputType Type)
{
	switch (Type)
	{
	case EVertexInputType::Default:
		return DefaultDeclaration;
	case EVertexInputType::PositionOnly:
		return PositionDeclaration;
	case EVertexInputType::PositionAndNormal:
		return PositionAndNormalDeclaration;
	default:
		return nullptr;
	}
}

void WLocalVertexFactory::GetStreams(EVertexInputType VertexInputType, VertexInputStream& OutVertexStream)
{
	if (VertexInputType == EVertexInputType::Default)
	{
		OutVertexStream.Push(VertexInputStreamElement(0, Data.PositionComponent.pVertexBuffer->GetRHI(), Data.PositionComponent.Offset));
		OutVertexStream.Push(VertexInputStreamElement(1, Data.NormalComponent.pVertexBuffer->GetRHI(), Data.NormalComponent.Offset));
		OutVertexStream.Push(VertexInputStreamElement(2, Data.TangentComponent.pVertexBuffer->GetRHI(), Data.TangentComponent.Offset));
		OutVertexStream.Push(VertexInputStreamElement(3, Data.ColorComponent.pVertexBuffer->GetRHI(), Data.ColorComponent.Offset));
		OutVertexStream.Push(VertexInputStreamElement(4, Data.TexCoordsComponent[0].pVertexBuffer->GetRHI(), Data.TexCoordsComponent[0].Offset));
		OutVertexStream.Push(VertexInputStreamElement(5, Data.TexCoordsComponent[1].pVertexBuffer->GetRHI(), Data.TexCoordsComponent[1].Offset));
		OutVertexStream.Push(VertexInputStreamElement(6, Data.TexCoordsComponent[2].pVertexBuffer->GetRHI(), Data.TexCoordsComponent[2].Offset));
		OutVertexStream.Push(VertexInputStreamElement(7, Data.TexCoordsComponent[3].pVertexBuffer->GetRHI(), Data.TexCoordsComponent[3].Offset));
		OutVertexStream.Push(VertexInputStreamElement(8, Data.TexCoordsComponent[4].pVertexBuffer->GetRHI(), Data.TexCoordsComponent[4].Offset));
		OutVertexStream.Push(VertexInputStreamElement(9, Data.TexCoordsComponent[5].pVertexBuffer->GetRHI(), Data.TexCoordsComponent[5].Offset));
		OutVertexStream.Push(VertexInputStreamElement(10, Data.TexCoordsComponent[6].pVertexBuffer->GetRHI(), Data.TexCoordsComponent[6].Offset));
		OutVertexStream.Push(VertexInputStreamElement(11, Data.TexCoordsComponent[7].pVertexBuffer->GetRHI(), Data.TexCoordsComponent[7].Offset));
	}
	else if (VertexInputType == EVertexInputType::PositionOnly)
	{
		OutVertexStream.Push(VertexInputStreamElement(0, Data.PositionComponent.pVertexBuffer->GetRHI(), Data.PositionComponent.Offset));
	}
	else if (VertexInputType == EVertexInputType::PositionAndNormal)
	{
		OutVertexStream.Push(VertexInputStreamElement(0, Data.PositionComponent.pVertexBuffer->GetRHI(), Data.PositionComponent.Offset));
		OutVertexStream.Push(VertexInputStreamElement(1, Data.NormalComponent.pVertexBuffer->GetRHI(), Data.NormalComponent.Offset));
		OutVertexStream.Push(VertexInputStreamElement(2, Data.TangentComponent.pVertexBuffer->GetRHI(), Data.TangentComponent.Offset));
	}
}
