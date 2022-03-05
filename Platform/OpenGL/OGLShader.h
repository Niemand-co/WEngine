#pragma once
#include "Utils/Shader.h"
#include "Math/Matrix.h"

class OGLShader : public Shader
{
public:
	OGLShader();
	~OGLShader();

	void Use();

	uint32_t GetID();

	void SetMatrix4f(char* factor, Matrix4x4f val);

	void SetVector4f(char* factor, Matrix4x4f val);

	void SetVector3f(char* factor, Matrix4x4f val);

	void SetInt(char* factor, int val);

	void SetFloat(char* factor, float val);
private:
	int m_programID;
};