#include "OGLShader.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

OGLShader::OGLShader()
{
	m_programID = glCreateProgram();
}

OGLShader::~OGLShader()
{
}

void OGLShader::Use()
{
}

uint32_t OGLShader::GetID()
{
	return uint32_t();
}

void OGLShader::SetMatrix4f(char* factor, Matrix4x4f val)
{
}

void OGLShader::SetVector4f(char* factor, Matrix4x4f val)
{
}

void OGLShader::SetVector3f(char* factor, Matrix4x4f val)
{
}

void OGLShader::SetInt(char* factor, int val)
{
}

void OGLShader::SetFloat(char* factor, float val)
{
}
