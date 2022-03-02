#include "Window.h"
#include "Render/RenderAPI.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/OpenGL/OGLWindow.h"

Window* Window::Create(WindProc* proc)
{
	switch (RenderAPI::GetAPI())
	{
		case RenderAPI::API::None:
		{
			return new WindowsWindow(proc);
		}
		case RenderAPI::API::OpenGL:
		{
			return new OGLWindow(proc);
		}
	}
}