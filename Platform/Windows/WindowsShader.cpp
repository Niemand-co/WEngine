#include "WindowsShader.h"

WindowsShader* WindowsShader::cur_shader = nullptr;

WindowsShader::WindowsShader()
	: Model(Matrix4x4f::GetIdentityMatrix()), View(Matrix4x4f::GetIdentityMatrix()), Projection(Matrix4x4f::GetIdentityMatrix())
{
	ProjViewMatrix = Projection * View;
}

V2F WindowsShader::VertexShader(const Vertex& vert)
{
	V2F v2f;
	v2f.WorldPos = Model * Vec4(vert.position, 1.0f);
	v2f.ScreenPos = ProjViewMatrix * v2f.WorldPos;
	v2f.Normal = ClipDimension(Transpose(Inverse(Model))) * vert.normal;
	v2f.Color = vert.color;
	v2f.UV = vert.uv;
	return v2f;
}

std::vector<Vec4> WindowsShader::FragmentShader(const V2F& v2f)
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
	//result.push_back(Vec4(v2f.Color, 1.0f));
	result.push_back(v2f.ScreenPos);
	return result;
}

void WindowsShader::SetModel(Matrix4x4f model)
{
	Model = model;
}

void WindowsShader::SetView(Matrix4x4f view)
{
	View = view;
	ProjViewMatrix = Projection * View;
}

void WindowsShader::SetProjection(Matrix4x4f projection)
{
	Projection = projection;
	ProjViewMatrix = Projection * View;
}

void WindowsShader::BindShader(Shader* shader)
{
	cur_shader = static_cast<WindowsShader*>(shader);
}

void WindowsShader::SetLightPos(const Vec3& pos)
{
	LightPos = pos;
}

void WindowsShader::SetLightColor(const Vec3& color)
{
	LightColor = color;
}

void WindowsShader::SetCameraPos(const Vec3& pos)
{
	CameraPos = pos;
}