#pragma once
#include "Render/RenderContext.h"

class OGLWindow;

class OGLRenderContext : RenderContext
{
public:
	OGLRenderContext(Window* window);
	virtual ~OGLRenderContext() = default;

	virtual void Init() override;
	virtual void SetBufferNum(BUFFER_NUM num) override;
	virtual void SwapBuffer() override;
private:
	OGLWindow* m_window;
};