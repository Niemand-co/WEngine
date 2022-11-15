#include "pch.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Render/Descriptor/Public/RHIVertexInputDescriptor.h"

VertexBindingDescription* Vertex::m_bindingDescription = nullptr;

std::vector<VertexAttributeDescription*> Vertex::m_attributeDescriptions = std::vector<VertexAttributeDescription*>();

Vertex::Vertex(glm::vec3 position, glm::vec3 color, glm::vec3 normal, glm::vec2 uv)
	: Position(position), Color(color), Normal(normal), UV(uv)
{

}

Vertex::~Vertex()
{

}

void Vertex::GenerateVertexInputDescription()
{
	m_bindingDescription = (VertexBindingDescription*)WEngine::Allocator::Get()->Allocate(sizeof(VertexBindingDescription));
	VertexAttributeDescription* attributeDescription = (VertexAttributeDescription*)WEngine::Allocator::Get()->Allocate(4 * sizeof(VertexAttributeDescription));
	m_attributeDescriptions.resize(4);
	for (int i = 0; i < 4; ++i)
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
	m_attributeDescriptions[2]->format = Format::R32G32B32_SFloat;

	m_attributeDescriptions[3]->slot = 0;
	m_attributeDescriptions[3]->location = 3;
	m_attributeDescriptions[3]->offset = sizeof(Vector3) * 3;
	m_attributeDescriptions[3]->format = Format::R32G32_SFloat;
}

RHIVertexInputDescriptor Vertex::GetVertexInputDescriptor()
{
	if (m_bindingDescription == nullptr || m_attributeDescriptions.size() == 0)
	{
		GenerateVertexInputDescription();
	}

	RHIVertexInputDescriptor vertexInputDescriptor = {};
	{
		vertexInputDescriptor.bindingDescription = m_bindingDescription;
		vertexInputDescriptor.attributeDescriptionCount = m_attributeDescriptions.size();
		vertexInputDescriptor.pAttributeDescription = m_attributeDescriptions.data();
	}

	return vertexInputDescriptor;
}