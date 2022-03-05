#include "OGLWindow.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "OGLRenderContext.h"

OGLWindow::OGLWindow(WindProc* proc)
	: m_width(proc->width), m_height(proc->height)
{
	glfwInit();
	
	m_window = glfwCreateWindow(proc->width, proc->height, proc->title.c_str(), nullptr, nullptr);
	m_renderContext = new OGLRenderContext(this);
	m_renderContext->Init();
}

OGLWindow::~OGLWindow()
{
	glfwDestroyWindow(m_window);
}

void* OGLWindow::GetWindowHandle() const
{
	return m_window;
}

uint32_t OGLWindow::GetWidth() const
{
	return m_width;
}

uint32_t OGLWindow::GetHeight() const
{
	return m_height;
}

void OGLWindow::OnUpdate()
{
	glfwPollEvents();
	m_renderContext->SwapBuffer();
}
