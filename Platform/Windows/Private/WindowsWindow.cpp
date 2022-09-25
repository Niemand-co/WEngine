#include "pch.h"
#include "Windows/Public/WindowsWindow.h"
#include "Render/Public/Framebuffer.h"
#include "Windows/Public/WindowsRenderContext.h"
#include "Icons/resource.h"

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

WindowsWindow::WindowsWindow(WinProc* proc)
	: m_width(proc->width), m_height(proc->height)
{
	HICON hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON_WINDOW));

	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(0);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = WindowProc;
	wc.hIcon = hIcon;
	wc.hIconSm = hIcon;
	wc.hCursor = LoadCursor(wc.hInstance, IDC_ARROW);
	wc.lpszClassName = proc->name;
	wc.lpszMenuName = nullptr;

	if (RegisterClassEx(&wc) == FALSE)
	{
		std::cout << "Failed to register class" << std::endl;
	}

	m_window = CreateWindowEx(
		0,
		proc->name,
		"WEngine",
		WS_OVERLAPPEDWINDOW,
		0, 0, proc->width, proc->height,
		NULL,
		NULL,
		wc.hInstance,
		NULL);

	hdc = GetDC(m_window);
	ScreenHDC = CreateCompatibleDC(hdc);

	//m_renderContext = new WindowsRenderContext(this);
	//m_renderContext->Init();

	ShowWindow(m_window, SW_SHOW);
}

WindowsWindow::~WindowsWindow()
{

}

void WindowsWindow::DrawPixel(int x, int y, Vector4 color)
{
	SetPixel(hdc, x, m_height - y, RGB(color.r * 255, color.g * 255, color.b * 255));
}

void* WindowsWindow::GetHandle() const
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

void WindowsWindow::Update()
{
	if (PeekMessage(&msg, m_window, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}/*
	else
		m_renderContext->SwapBuffer();*/
}
