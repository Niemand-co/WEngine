//#pragma once
//#include "RHI/Public/RHIShader.h"
//
//template<typename T>
//struct Vector3Type;
//template<typename T>
//struct Vector4Type;
//struct V2F;
//
//typedef Vector3Type<float> Vector3;
//typedef Vector4Type<float> Vector4;
//
//class WindowsShader : public RHIShader
//{
//public:
//	WindowsShader();
//	virtual ~WindowsShader() = default;
//
//	V2F VertexShader(const Vertex& vert);
//	std::vector<Vector4> FragmentShader(const V2F& v2f);
//
//	void SetModel(Matrix4x4f model);
//	void SetView(Matrix4x4f view);
//	void SetProjection(Matrix4x4f projection);
//
//	static WindowsShader* cur_shader;
//	static void BindShader(RHIShader* shader);
//
//	void SetLightPos(const Vector3& pos);
//	void SetLightColor(const Vector3& color);
//	void SetCameraPos(const Vector3& pos);
//private:
//	Matrix4x4f Model;
//	Matrix4x4f View;
//	Matrix4x4f Projection;
//	Matrix4x4f ProjViewMatrix;
//	Vector3 LightPos;
//	Vector3 LightColor;
//	Vector3 CameraPos;
//};