#include "OGLRenderContext.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "OpenGL/OGLWindow.h"

#include <iostream>

OGLRenderContext::OGLRenderContext(Window* window)
	: m_window(static_cast<OGLWindow*>(window))
{
}

void OGLRenderContext::Init()
{
	glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_window->GetWindowHandle()));
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout<<"Failed to Load GLAD!"<<std::endl;
	}
}

void OGLRenderContext::SetBufferNum(BUFFER_NUM num)
{
}

void OGLRenderContext::SwapBuffer()
{
	glfwSwapBuffers(static_cast<GLFWwindow*>(m_window->GetWindowHandle()));
}
