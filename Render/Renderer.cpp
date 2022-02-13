#include "Renderer.h"

Renderer::Renderer(Primitive type = Primitive::TRIANGLE)
	: m_type(type), m_shader(nullptr)
{
	m_rasterizer = new Rasterizer();
}

void Renderer::SetShader(Shader* shader)
{
	m_shader = shader;
}

void Renderer::DrawByIndices(Entity* entity)
{
	int step = 3;
	if(m_type == Primitive::LINE)
		step = 2;
	else if(m_type == Primitive::POINT)
		step = 1;

	m_rasterizer->SetPixelShader(m_shader);

	std::vector<Vertex>& vertices = entity->GetMesh()->GetVertices();
	std::vector<uint32_t>& indices = entity->GetMesh()->GetIndices();
	for (int i = 0; i <= indices.size() - step; i += step)
	{
		std::vector<V2F> primitive_vertices(step);
		for (int vert_id = 0; vert_id < step; ++vert_id)
		{
			primitive_vertices[vert_id] = m_shader->VertexShader(vertices[indices[int(long long(vert_id) + long long(i))]]);
		}
		if(m_type == Primitive::TRIANGLE)
			m_rasterizer->RasterizeTriangle(primitive_vertices[0], primitive_vertices[1], primitive_vertices[2]);
		else if(m_type == Primitive::LINE)
			m_rasterizer->RasterizeLine(primitive_vertices[0], primitive_vertices[1]);
		else if(m_type == Primitive::POINT)
			m_rasterizer->RasterizePoint(primitive_vertices[0]);
	}
}