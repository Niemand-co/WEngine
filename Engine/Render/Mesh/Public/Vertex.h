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

struct Vertex
{
public:

	Vertex(Vector3 position = {0, 0, 0}, Vector3 color = {0, 0, 0}, Vector2 uv = {0, 0});

	~Vertex();

public:

	Vector3 Position;

	Vector3 Color;

	Vector2 UV;
	
};