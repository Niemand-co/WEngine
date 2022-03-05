#include "Shader.h"
#include "Platform/OpenGL/OGLShader.h"
#include "Platform/Windows/WindowsShader.h"
#include "Render/RenderAPI.h"

std::vector<Shader*> ShaderLibrary::m_shaders;

Shader::~Shader()
{

}

Shader* ShaderLibrary::Allocate()
{
	switch (RenderAPI::GetAPI())
	{
		case RenderAPI::API::None:
		{
			Shader* shader = new WindowsShader();
			m_shaders.emplace_back(shader);
			return shader;
		}
		case RenderAPI::API::OpenGL:
		{
			Shader* shader = new OGLShader();
			m_shaders.emplace_back(shader);
			return shader;
		}
		default:
		{
			std::cout<<"Error Type!"<<std::endl;
			exit(0);
		}
	}
}