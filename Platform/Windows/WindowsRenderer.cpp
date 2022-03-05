#include "WindowsRenderer.h"
#include "Render/Rasterizer.h"
#include "WindowsShader.h"
#include "Scene/Light.h"
#include "Camera/Camera.h"

WindowsRenderer::WindowsRenderer(Primitive type)
{
	m_type = type;
	m_rasterizer = new Rasterizer();
}

WindowsRenderer::~WindowsRenderer()
{
}

void WindowsRenderer::DrawByIndices(Entity* entity, Light* light, Camera* camera)
{
	int step = 3;
	if (m_type == Primitive::LINE)
		step = 2;
	else if (m_type == Primitive::POINT)
		step = 1;

	WindowsShader::BindShader(entity->GetMaterial()->GetShader());
	WindowsShader::cur_shader->SetLightColor(light->GetColor());
	WindowsShader::cur_shader->SetLightPos(static_cast<PointLight*>(light)->GetPos());
	WindowsShader::cur_shader->SetCameraPos(camera->GetPos());
	WindowsShader::cur_shader->SetModel(entity->GetTransformer()->GetModelMatrix());

	std::vector<Vertex>& vertices = entity->GetMesh()->GetVertices();
	std::vector<uint32_t>& indices = entity->GetMesh()->GetIndices();
	for (int i = 0; i <= indices.size() - step; i += step)
	{
		std::vector<V2F> primitive_vertices(step);
		for (int vert_id = 0; vert_id < step; ++vert_id)
		{
			primitive_vertices[vert_id] = WindowsShader::cur_shader->VertexShader(vertices[indices[int(long long(vert_id) + long long(i))]]);
		}
		if (m_type == Primitive::TRIANGLE)
			m_rasterizer->RasterizeTriangle(primitive_vertices[0], primitive_vertices[1], primitive_vertices[2]);
		else if (m_type == Primitive::LINE)
			m_rasterizer->RasterizeLine(primitive_vertices[0], primitive_vertices[1]);
		else if (m_type == Primitive::POINT)
			m_rasterizer->RasterizePoint(primitive_vertices[0]);
	}
}
