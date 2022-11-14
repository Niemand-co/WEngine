#include "pch.h"
#include "Render/Mesh/Public/Vertex.h"

Vertex::Vertex(glm::vec3 position, glm::vec3 color, glm::vec3 normal, glm::vec2 uv)
	: Position(position), Color(color), Normal(normal), UV(uv)
{

}

Vertex::~Vertex()
{

}