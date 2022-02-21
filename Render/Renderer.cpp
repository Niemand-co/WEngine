#include "Renderer.h"
#include <thread>

std::vector<Renderer> RendererLibrary::m_renderers;

Renderer::Renderer(Primitive type = Primitive::TRIANGLE)
	: m_type(type)
{
	m_rasterizer = new Rasterizer();
}

void Renderer::DrawByIndices(Entity* entity, Light* light, Camera* camera)
{
	int step = 3;
	if(m_type == Primitive::LINE)
		step = 2;
	else if(m_type == Primitive::POINT)
		step = 1;

	Shader::BindShader(entity->GetMaterial()->GetShader());
	Shader::cur_shader->SetLightColor(light->GetColor());
	Shader::cur_shader->SetLightPos(static_cast<PointLight*>(light)->GetPos());
	Shader::cur_shader->SetCameraPos(camera->GetPos());
	Shader::cur_shader->SetModel(entity->GetTransformer()->GetModelMatrix());

	std::vector<Vertex>& vertices = entity->GetMesh()->GetVertices();
	std::vector<uint32_t>& indices = entity->GetMesh()->GetIndices();
	for (int i = 0; i <= indices.size() - step; i += step)
	{
		std::vector<V2F> primitive_vertices(step);
		for (int vert_id = 0; vert_id < step; ++vert_id)
		{
			primitive_vertices[vert_id] = Shader::cur_shader->VertexShader(vertices[indices[int(long long(vert_id) + long long(i))]]);
		}
		if(m_type == Primitive::TRIANGLE)
			m_rasterizer->RasterizeTriangle(primitive_vertices[0], primitive_vertices[1], primitive_vertices[2]);
		else if(m_type == Primitive::LINE)
			m_rasterizer->RasterizeLine(primitive_vertices[0], primitive_vertices[1]);
		else if(m_type == Primitive::POINT)
			m_rasterizer->RasterizePoint(primitive_vertices[0]);
	}
}

Renderer* RendererLibrary::Allocate(Renderer::Primitive type)
{
	m_renderers.emplace_back(type);
	return &m_renderers[m_renderers.size() - 1];
}