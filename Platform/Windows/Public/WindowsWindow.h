#pragma once
#include "Utils/Public/Window.h"

template<typename T>
struct Vector4Type;
class Framebuffer;
class WindowsRenderContext;

typedef Vector4Type<float> Vector4;

class WindowsWindow : public Window
{
public:
	WindowsWindow(WinProc* proc);
	~WindowsWindow();

	void DrawPixel(int x, int y, Vector4 color);

	void Init() override {}
	void Destroy() override {}
	void Update() override;

	virtual void* GetHandle() const override;
	virtual uint32_t GetWidth() const override;
	virtual uint32_t GetHeight() const override;

	HDC ScreenHDC;
	HDC hdc;
private:
	uint32_t m_width;
	uint32_t m_height;
	HWND m_window;
	MSG msg;
	WindowsRenderContext* m_renderContext;
};