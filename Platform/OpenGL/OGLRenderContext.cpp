#include "OGLRenderContext.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

OGLRenderContext::OGLRenderContext(GLFWwindow* window)
	: m_window(window)
{
}

void OGLRenderContext::Init()
{
	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout<<"Failed to Load GLAD!"<<std::endl;
	}
}

void OGLRenderContext::SwapBuffer()
{
	glfwSwapBuffers(m_window);
}
