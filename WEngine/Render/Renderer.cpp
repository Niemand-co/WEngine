#include "Renderer.h"
#include "RenderAPI.h"
#include "Platform/Windows/WindowsRenderer.h"
#include "Platform/OpenGL/OGLRenderer.h"

std::vector<Renderer*> RendererLibrary::m_renderers;

Renderer* RendererLibrary::Allocate(Renderer::Primitive type)
{
	switch (RenderAPI::GetAPI())
	{
		case RenderAPI::API::None:
		{
			Renderer* renderer = new WindowsRenderer(type);
			m_renderers.emplace_back(renderer);
			return renderer;
		}
		case RenderAPI::API::OpenGL:
		{
			Renderer* renderer = new OGLRenderer(type);
			m_renderers.emplace_back(renderer);
			return renderer;
		}
		default:
		{
			std::cout<<"Error Type!"<<std::endl;
			exit(0);
		}
	}
}