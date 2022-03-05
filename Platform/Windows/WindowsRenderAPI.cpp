#include "WindowsRenderAPI.h"
#include "Render/Framebuffer.h"
#include "Utils/Window.h"
#include <windows.h>

void WindowsRenderAPI::Init()
{
}

void WindowsRenderAPI::SetClearColor(Vec4 Color)
{
	Framebuffer::SetClearColor(Color);
}

void WindowsRenderAPI::Clear()
{
	Framebuffer::ClearBuffer(COLOR_BUFFER | DEPTH_BUFFER);
}

void WindowsRenderAPI::SetViewport(const uint32_t& x, const uint32_t& y, const uint32_t& width, const uint32_t& height)
{
	MoveWindow(static_cast<HWND>(Window::cur_window->GetWindowHandle()), x, y, width, height, true);
}
