#pragma once
#include "Utils/Window.h"
#include "Math/Vector.h"
#include <stdint.h>
#include <windows.h>
#include <iostream>
#include <vector>

class WindowsWindow : public Window
{
public:
	WindowsWindow(WindProc* proc);
	~WindowsWindow();

	void DrawPixel(int x, int y, Vec4 color);

	virtual void* GetWindowHandle() const override;
	virtual uint32_t GetWidth() const override;
	virtual uint32_t GetHeight() const override;

	virtual void OnUpdate() override;

	HDC ScreenHDC;
	HDC hdc;
private:
	uint32_t m_width;
	uint32_t m_height;
	HWND m_window;
	MSG msg;
};