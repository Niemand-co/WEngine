#include "pch.h"
#include "Render/Mesh/Public/Vertex.h"

Vertex::Vertex(Vector3 position, Vector3 color, Vector3 normal, Vector2 uv)
	: Position(position), Color(color), Normal(normal), UV(uv)
{

}

Vertex::~Vertex()
{

}