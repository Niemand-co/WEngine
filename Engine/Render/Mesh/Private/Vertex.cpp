#include "pch.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Render/Descriptor/Public/RHIVertexInputDescriptor.h"
#include "Render/Public/Buffer.h"

//void Vertex::GenerateVertexInputDescription()
//{
//	m_bindingDescription = (VertexBindingDescription*)NormalAllocator::Get()->Allocate(sizeof(VertexBindingDescription));
//	VertexAttributeDescription* attributeDescription = (VertexAttributeDescription*)NormalAllocator::Get()->Allocate(4 * sizeof(VertexAttributeDescription));
//	m_attributeDescriptions.Resize(4);
//	for (int i = 0; i < 4; ++i)
//	{
//		m_attributeDescriptions[i] = attributeDescription + i;
//	}
//
//	m_bindingDescription->slot = 0;
//	m_bindingDescription->stride = sizeof(Vertex);
//
//	m_attributeDescriptions[0]->slot = 0;
//	m_attributeDescriptions[0]->location = 0;
//	m_attributeDescriptions[0]->offset = 0;
//	m_attributeDescriptions[0]->format = Format::R32G32B32_SFloat;
//
//	m_attributeDescriptions[1]->slot = 0;
//	m_attributeDescriptions[1]->location = 1;
//	m_attributeDescriptions[1]->offset = sizeof(Vector3);
//	m_attributeDescriptions[1]->format = Format::R32G32B32_SFloat;
//
//	m_attributeDescriptions[2]->slot = 0;
//	m_attributeDescriptions[2]->location = 2;
//	m_attributeDescriptions[2]->offset = sizeof(Vector3) * 2;
//	m_attributeDescriptions[2]->format = Format::R32G32B32_SFloat;
//
//	m_attributeDescriptions[3]->slot = 0;
//	m_attributeDescriptions[3]->location = 3;
//	m_attributeDescriptions[3]->offset = sizeof(Vector3) * 3;
//	m_attributeDescriptions[3]->format = Format::R32G32_SFloat;
//}
//
//RHIVertexInputDescriptor Vertex::GetVertexInputDescriptor()
//{
//	if (m_bindingDescription == nullptr || m_attributeDescriptions.Size() == 0)
//	{
//		GenerateVertexInputDescription();
//	}
//
//	RHIVertexInputDescriptor vertexInputDescriptor = {};
//	{
//		vertexInputDescriptor.bindingDescription = m_bindingDescription;
//		vertexInputDescriptor.attributeDescriptionCount = m_attributeDescriptions.Size();
//		vertexInputDescriptor.pAttributeDescription = m_attributeDescriptions.GetData();
//	}
//
//	return vertexInputDescriptor;
//}

WVertexFactory::WVertexFactory()
{
}

WVertexFactory::~WVertexFactory()
{
}

void WVertexFactory::InitRHIResource()
{
	//m_pVertexBuffer->InitRHIResource();
}

void WVertexFactory::ReleaseRHIResource()
{
	//m_pVertexBuffer->ReleaseRHIResource();
}

void WVertexFactory::UpdateRHIResource()
{
	//m_pVertexBuffer->UpdateRHIResource();
}

void WLocalVertexFactory::GetPSOVertexInputElements(EInputType Type, WEngine::WArray<VertexInputElement>& Elements)
{
	Elements.Push(VertexInputElement(0, 0, 12, VertexElementType::VET_Float3, false));

	switch (Type)
	{
	case EInputType::Default:
		Elements.Push(VertexInputElement(13, 0, 0, VertexElementType::VET_UInt, true));
		break;
	case EInputType::PositionOnly:
		Elements.Push(VertexInputElement(1, 0, 0, VertexElementType::VET_UInt, true));
		break;
	case EInputType::PositionAndNormal:
		Elements.Push(VertexInputElement(1, 4, 0, VertexElementType::VET_PackedNormal, false));
		Elements.Push(VertexInputElement(2, 0, 0, VertexElementType::VET_UInt, true));
		break;
	default:
		break;
	}
}
