#include "Shader.h"
#include <math.h>

std::vector<Shader> ShaderLibrary::m_shaders;
Shader* Shader::cur_shader = nullptr;

Shader::Shader()
	: Model(Matrix4x4f::GetIdentityMatrix()), View(Matrix4x4f::GetIdentityMatrix()), Projection(Matrix4x4f::GetIdentityMatrix())
{
	ProjViewMatrix = Projection * View;
}

Shader::Shader(Matrix4x4f view, Matrix4x4f projection)
	: Model(Matrix4x4f::GetIdentityMatrix()), View(view), Projection(projection)
{
	ProjViewMatrix = projection * view;
}

V2F Shader::VertexShader(const Vertex& vert)
{
	V2F v2f;
	v2f.WorldPos = Model * Vec4(vert.position, 1.0f);
	v2f.ScreenPos = ProjViewMatrix * v2f.WorldPos;
	v2f.Normal = ClipDimension(Transpose(Inverse(Model))) * vert.normal;
	v2f.Color = vert.color;
	v2f.UV = vert.uv;
	return v2f;
}

std::vector<Vec4> Shader::FragmentShader(const V2F& v2f)
{
	std::vector<Vec4> result;
	Vec3 lightDir = (LightPos - Vec3(v2f.WorldPos)).Norm();
	Vec3 viewDir = (CameraPos - Vec3(v2f.WorldPos)).Norm();
	Vec3 normal = Vec3(v2f.Normal).Norm();

	float diffuse = std::max(DotProduct(lightDir, normal), 0.0f);
	Vec3 diffuse_light = diffuse * LightColor;

	Vec3 halfDir = (viewDir + lightDir).Norm();
	float specular = std::pow(std::max(DotProduct(halfDir, normal), 0.0f), 8);

	Vec3 color = v2f.Color * 0.2 + v2f.Color * diffuse_light + specular * LightColor * v2f.Color;

	result.push_back(Vec4(color, 1.0f));
	result.push_back(v2f.ScreenPos);
	return result;
}

void Shader::SetModel(Matrix4x4f model)
{
	Model = model;
}

void Shader::SetView(Matrix4x4f view)
{
	View = view;
	ProjViewMatrix = Projection * View;
}

void Shader::SetProjection(Matrix4x4f projection)
{
	Projection = projection;
	ProjViewMatrix = Projection * View;
}

void Shader::BindShader(Shader* shader)
{
	cur_shader = shader;
}

void Shader::SetLightPos(const Vec3& pos)
{
	LightPos = pos;
}

void Shader::SetLightColor(const Vec3& color)
{
	LightColor = color;
}

void Shader::SetCameraPos(const Vec3& pos)
{
	CameraPos = pos;
}

Shader* ShaderLibrary::Allocate()
{
	m_shaders.emplace_back(Shader());
	return &m_shaders[m_shaders.size() - 1];
}

Shader* ShaderLibrary::Allocate(Matrix4x4f view, Matrix4x4f projection)
{
	m_shaders.emplace_back(view, projection);
	return &m_shaders[m_shaders.size() - 1];
}