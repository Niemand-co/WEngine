#pragma once
#include "Render/RenderContext.h"

struct GLFWwindow;

class OGLRenderContext : RenderContext
{
public:
	OGLRenderContext(GLFWwindow* window);

	virtual void Init() override;
	virtual void SwapBuffer() override;
private:
	GLFWwindow* m_window;
};