#pragma once
#include "Render/RenderContext.h"
#include <windows.h>

class Framebuffer;
class WindowsWindow;

class WindowsRenderContext : public RenderContext
{
public:
	WindowsRenderContext(Window* window);
	virtual ~WindowsRenderContext() = default;

	virtual void Init() override;
	virtual void SetBufferNum(BUFFER_NUM num) override;
	virtual void SwapBuffer() override;
public:
	WindowsWindow* m_window;
	Framebuffer* m_famebuffers[2];
	int m_bufferNum;
	int cur_buffer;
};