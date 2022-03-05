#pragma once
#include "Utils/Shader.h"

class WindowsShader : public Shader
{
public:
	WindowsShader();
	virtual ~WindowsShader() = default;

	V2F VertexShader(const Vertex& vert);
	std::vector<Vec4> FragmentShader(const V2F& v2f);

	void SetModel(Matrix4x4f model);
	void SetView(Matrix4x4f view);
	void SetProjection(Matrix4x4f projection);

	static WindowsShader* cur_shader;
	static void BindShader(Shader* shader);

	void SetLightPos(const Vec3& pos);
	void SetLightColor(const Vec3& color);
	void SetCameraPos(const Vec3& pos);
private:
	Matrix4x4f Model;
	Matrix4x4f View;
	Matrix4x4f Projection;
	Matrix4x4f ProjViewMatrix;
	Vec3 LightPos;
	Vec3 LightColor;
	Vec3 CameraPos;
};