#pragma once
#ifndef SR_SHADER_H
#define SR_SHADER_H
#include "Math/Matrix.h"
#include "Renderer/Vertex.h"

class Shader
{
public:
	Shader();
	Shader(Matrix4x4f model, Matrix4x4f view, Matrix4x4f projection);
	~Shader() = default;

	V2F VertexShader(const Vertex& vert);
	Vec4 FragmentShader(const V2F& v2f);

private:
	Matrix4x4f Model;
	Matrix4x4f View;
	Matrix4x4f Projection;
};

#endif