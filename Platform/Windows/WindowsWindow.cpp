#include "WindowsWindow.h"
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
		std::cout << char(wParam) << std::endl;
		break;
	}
	}

	return DefWindowProcA(hWnd, msg, wParam, lParam);
}

WindowsWindow::WindowsWindow(WindProc* proc)
	: m_width(proc->width), m_height(proc->height)
{
	WNDCLASS wc = {};
	wc.hInstance = GetModuleHandle(0);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wc.lpszClassName = proc->title.c_str();

	if (RegisterClass(&wc) == FALSE)
	{
		std::cout << "Failed to register class" << std::endl;
	}

	m_window = CreateWindowEx(
		0,
		proc->title.c_str(),
		"Soft Renderer Window",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, proc->width, proc->height,
		NULL,
		NULL,
		wc.hInstance,
		NULL);

	hdc = GetDC(m_window);
	ScreenHDC = CreateCompatibleDC(hdc);

	ShowWindow(m_window, SW_SHOW);
	UpdateWindow(m_window);
}

WindowsWindow::~WindowsWindow()
{

}

void WindowsWindow::DrawPixel(int x, int y, Vec4 color)
{
	SetPixel(hdc, x, m_height - y, RGB(color.r * 255, color.g * 255, color.b * 255));
}

void* WindowsWindow::GetWindowHandle() const
{
	return m_window;
}

uint32_t WindowsWindow::GetWidth() const
{
	return m_width;
}

uint32_t WindowsWindow::GetHeight() const
{
	return m_height;
}

void WindowsWindow::OnUpdate()
{
	if (GetMessage(&msg, m_window, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
