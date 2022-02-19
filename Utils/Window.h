#pragma once
#ifndef SR_WINDOW_H
#define SR_WINDOW_H
#include "Math/Vector.h"
#include <stdint.h>
#include <windows.h>
#include <iostream>
#include <vector>

class Window
{
public:
	Window(const char* name, uint32_t width, uint32_t height);
	~Window();

	void DrawPixel(int x, int y, Vec4 color);

	HWND GetWindowHandle();
	uint32_t GetWidth();
	uint32_t GetHeight();

	HDC ScreenHDC;
	HDC hdc;
private:
	uint32_t m_width;
	uint32_t m_height;
	HWND m_window;
};
#endif