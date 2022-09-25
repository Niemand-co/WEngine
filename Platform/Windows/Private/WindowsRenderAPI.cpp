#include "pch.h"
#include "Windows/Public/WindowsRenderAPI.h"
#include "Render/Public/Framebuffer.h"
#include "Utils/Public/Window.h"

void WindowsRenderAPI::Init()
{
}

void WindowsRenderAPI::SetClearColor(Vector4 Color)
{
	Framebuffer::SetClearColor(Color);
}

void WindowsRenderAPI::Clear()
{
	Framebuffer::ClearBuffer(COLOR_BUFFER | DEPTH_BUFFER);
}

void WindowsRenderAPI::SetViewport(const uint32_t& x, const uint32_t& y, const uint32_t& width, const uint32_t& height)
{
	MoveWindow(static_cast<HWND>(Window::cur_window->GetHandle()), x, y, width, height, true);
}
