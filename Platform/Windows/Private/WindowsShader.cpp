//#include "pch.h"
//#include "Windows/Public/WindowsShader.h"
//#include "Render/Public/Vertex.h"
//
//WindowsShader* WindowsShader::cur_shader = nullptr;
//
//WindowsShader::WindowsShader()
//	: Model(Matrix4x4f::GetIdentityMatrix()), View(Matrix4x4f::GetIdentityMatrix()), Projection(Matrix4x4f::GetIdentityMatrix())
//{
//	ProjViewMatrix = Projection * View;
//}
//
//V2F WindowsShader::VertexShader(const Vertex& vert)
//{
//	V2F v2f;
//	v2f.WorldPos = Model * Vector4(vert.position, 1.0f);
//	v2f.ScreenPos = ProjViewMatrix * v2f.WorldPos;
//	v2f.Normal = ClipDimension(Transpose(Inverse(Model))) * vert.normal;
//	v2f.Color = vert.color;
//	v2f.UV = vert.uv;
//	return v2f;
//}
//
//std::vector<Vector4> WindowsShader::FragmentShader(const V2F& v2f)
//{
//	std::vector<Vector4> result;
//	Vector3 lightDir = (LightPos - Vector3(v2f.WorldPos)).Norm();
//	Vector3 viewDir = (CameraPos - Vector3(v2f.WorldPos)).Norm();
//	Vector3 normal = Vector3(v2f.Normal).Norm();
//
//	float diffuse = Saturate(DotProduct(lightDir, normal));
//	Vector3 diffuse_light = diffuse * LightColor;
//
//	Vector3 halfDir = (viewDir + lightDir).Norm();
//	float specular = std::pow(Saturate(DotProduct(halfDir, normal)), 8);
//
//	Vector3 color = v2f.Color * 0.2 + v2f.Color * diffuse_light + specular * LightColor * v2f.Color;
//
//	result.push_back(Vector4(color, 1.0f));
//	//result.push_back(Vec4(v2f.Color, 1.0f));
//	result.push_back(v2f.ScreenPos);
//	return result;
//}
//
//void WindowsShader::SetModel(Matrix4x4f model)
//{
//	Model = model;
//}
//
//void WindowsShader::SetView(Matrix4x4f view)
//{
//	View = view;
//	ProjViewMatrix = Projection * View;
//}
//
//void WindowsShader::SetProjection(Matrix4x4f projection)
//{
//	Projection = projection;
//	ProjViewMatrix = Projection * View;
//}
//
//void WindowsShader::BindShader(RHIShader* shader)
//{
//	cur_shader = static_cast<WindowsShader*>(shader);
//}
//
//void WindowsShader::SetLightPos(const Vector3& pos)
//{
//	LightPos = pos;
//}
//
//void WindowsShader::SetLightColor(const Vector3& color)
//{
//	LightColor = color;
//}
//
//void WindowsShader::SetCameraPos(const Vector3& pos)
//{
//	CameraPos = pos;
//}