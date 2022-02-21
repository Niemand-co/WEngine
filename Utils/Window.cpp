#include "Window.h"
#include <iostream>

LRESULT WINAPI WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY:
		{
			exit(0);
		}
		case WM_KEYDOWN:
		{
			std::cout<<char(wParam)<<std::endl;
			break;
		}
	}

	return DefWindowProcA(hWnd, msg, wParam, lParam);
}

Window::Window(const char* name, uint32_t width, uint32_t height)
 : m_width(width), m_height(height)
{
	WNDCLASS wc = {};
	wc.hInstance = GetModuleHandle(0);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wc.lpszClassName = name;

	if (RegisterClass(&wc) == FALSE)
	{
		std::cout<<"Failed to register class"<<std::endl;
	}

	m_window = CreateWindowEx(
				0,
				name,
				"Soft Renderer Window",
				WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
				0, 0, width, height,
				NULL,
				NULL,
				wc.hInstance,
				NULL);

	hdc = GetDC(m_window);
	ScreenHDC = CreateCompatibleDC(hdc);

	ShowWindow(m_window, SW_SHOW);
	UpdateWindow(m_window);
}

Window::~Window()
{

}

void Window::DrawPixel(int x, int y, Vec4 color)
{
	SetPixel(hdc, x, m_height - y, RGB(color.r * 255, color.g * 255, color.b * 255));
}

HWND Window::GetWindowHandle()
{
	return m_window;
}

uint32_t Window::GetWidth()
{
	return m_width;
}

uint32_t Window::GetHeight()
{
	return m_height;
}