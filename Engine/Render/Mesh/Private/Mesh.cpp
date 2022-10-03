#include "pch.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Render/Descriptor/Public/RHIVertexInputDescriptor.h"

Mesh::Mesh()
{
	m_pVertices = nullptr;
	m_vertexCount = 0;
	m_pIndices = nullptr;
	m_indexCount = 0;
	m_bindingDescription = nullptr;
}

Mesh::~Mesh()
{
}

void Mesh::GenerateVertexInputDescription()
{
	m_bindingDescription = (VertexBindingDescription*)WEngine::Allocator::Get()->Allocate(sizeof(VertexBindingDescription));
	VertexAttributeDescription *attributeDescription = (VertexAttributeDescription*)WEngine::Allocator::Get()->Allocate(3 * sizeof(VertexAttributeDescription));
	m_attributeDescriptions.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		m_attributeDescriptions[i] = attributeDescription + i;
	}

	m_bindingDescription->slot = 0;
	m_bindingDescription->stride = sizeof(Vertex);

	m_attributeDescriptions[0]->slot = 0;
	m_attributeDescriptions[0]->location = 0;
	m_attributeDescriptions[0]->offset = 0;
	m_attributeDescriptions[0]->format = Format::R32G32B32_SFloat;

	m_attributeDescriptions[1]->slot = 0;
	m_attributeDescriptions[1]->location = 1;
	m_attributeDescriptions[1]->offset = sizeof(Vector3);
	m_attributeDescriptions[1]->format = Format::R32G32B32_SFloat;

	m_attributeDescriptions[2]->slot = 0;
	m_attributeDescriptions[2]->location = 2;
	m_attributeDescriptions[2]->offset = sizeof(Vector3) * 2;
	m_attributeDescriptions[2]->format = Format::R32G32_SFloat;
}

RHIVertexInputDescriptor Mesh::GetVertexInputDescriptor()
{
	if (m_bindingDescription == nullptr || m_attributeDescriptions.size() == 0)
	{
		GenerateVertexInputDescription();
	}

	RHIVertexInputDescriptor vertexInputDescriptor = {};
	{
		vertexInputDescriptor.bindingDescription = m_bindingDescription;
		vertexInputDescriptor.attributeDescriptionCount = 3;
		vertexInputDescriptor.pAttributeDescription = m_attributeDescriptions.data();
	}

	return vertexInputDescriptor;
}
