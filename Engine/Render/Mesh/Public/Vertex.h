#pragma once

struct VertexBindingDescription
{
	unsigned int slot;
	size_t stride;
};

struct VertexAttributeDescription
{
	unsigned int slot;
	unsigned int location;
	unsigned int offset;
	Format format;
};

class RHIVertexInputDescriptor;

struct Vertex
{
public:

	Vertex(glm::vec3 position = {0, 0, 0}, glm::vec3 color = {0, 0, 0}, glm::vec3 normal = {1.0f, 0, 0}, glm::vec2 uv = {0, 0});

	~Vertex();

	static void GenerateVertexInputDescription();

	static RHIVertexInputDescriptor GetVertexInputDescriptor();

public:

	glm::vec3 Position;

	glm::vec3 Color;

	glm::vec3 Normal;

	glm::vec2 UV;

private:

	static VertexBindingDescription* m_bindingDescription;

	static std::vector<VertexAttributeDescription*> m_attributeDescriptions;
	
};