#include "pch.h"
#include "Utils/Public/Window.h"
#include "Platform/Windows/Public/WindowsWindow.h"
#include "Platform/GLFWWindow/Public/GLFWWindow.h"

Window* Window::cur_window = nullptr;

bool Window::GetIsClosed()
{
	return m_isClosed;
}

bool Window::GetShouldClose()
{
	return m_shouldClose;
}

void Window::SetShouldClose(bool shouldClose)
{
	m_shouldClose = shouldClose;
}

Window* Window::Get()
{
	if (cur_window != nullptr)
		return cur_window;

	WinProc* proc = new WinProc();

	//switch (RenderAPI::GetAPI())
	//{
	//	case RenderAPI::API::None:
	//	{
			cur_window = (Window*)WEngine::Allocator::Get()->Allocate(sizeof(GLFWWindow));
			::new (cur_window) GLFWWindow(proc);
			return cur_window;
	//	}
	//	default:
	//	{
	//		RE_ASSERT(false, "Error Tyoe System.");
	//	}
	//	return nullptr;
	//}
}

Window* Window::Get(WinProc* proc)
{
	if(cur_window != nullptr)
		return cur_window;

	//switch (RenderAPI::GetAPI())
	//{
	//	case RenderAPI::API::None:
	//	{
			cur_window = (Window*)WEngine::Allocator::Get()->Allocate(sizeof(GLFWWindow));
			::new (cur_window) GLFWWindow(proc);
			return cur_window;
	//	}
	//}
}