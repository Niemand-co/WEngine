#include "Window.h"
#include "Render/RenderAPI.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/OpenGL/OGLWindow.h"

Window* Window::cur_window = nullptr;

Window* Window::Create(WindProc* proc)
{
	switch (RenderAPI::GetAPI())
	{
		case RenderAPI::API::None:
		{
			cur_window = new WindowsWindow(proc);
			return cur_window;
		}
		case RenderAPI::API::OpenGL:
		{
			cur_window = new OGLWindow(proc);
			return cur_window;
		}
	}
}