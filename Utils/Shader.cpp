#include "Shader.h"

Shader::Shader()
	: Model(Matrix4x4f::GetIdentityMatrix()), View(Matrix4x4f::GetIdentityMatrix()), Projection(Matrix4x4f::GetIdentityMatrix())
{
}

Shader::Shader(Matrix4x4f model, Matrix4x4f view, Matrix4x4f projection)
	: Model(model), View(view), Projection(projection)
{
}

V2F Shader::VertexShader(const Vertex& vert)
{
	V2F v2f;
	v2f.WorldPos = Model * Vec4(vert.position, 1.0f);
	v2f.ScreenPos = Projection * View * v2f.WorldPos;
	v2f.Normal = ClipDimension(Transpose(Inverse(Model))) * vert.normal;
	v2f.Color = vert.color;
	v2f.UV = vert.uv;
	return v2f;
}

Vec4 Shader::FragmentShader(const V2F& v2f)
{
	return Vec4(v2f.Color, 1.0f);
}