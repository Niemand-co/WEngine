#pragma once
#include "Utils/Window.h"

struct GLFWwindow;
class OGLRenderContext;

class OGLWindow : public Window
{
public:
	OGLWindow(WindProc* proc);
	~OGLWindow();

	virtual void* GetWindowHandle() const override;
	virtual uint32_t GetWidth() const override;
	virtual uint32_t GetHeight() const override;

	virtual void OnUpdate() override;
private:
	uint32_t m_width;
	uint32_t m_height;
	GLFWwindow* m_window;
	OGLRenderContext* m_renderContext;
};