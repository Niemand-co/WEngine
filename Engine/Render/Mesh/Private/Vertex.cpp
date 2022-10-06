#include "pch.h"
#include "Render/Mesh/Public/Vertex.h"

Vertex::Vertex(Vector3 position, Vector3 color, Vector2 uv)
	: Position(position), Color(color), UV(uv)
{

}

Vertex::~Vertex()
{

}