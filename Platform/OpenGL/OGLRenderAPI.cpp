#include "OGLRenderAPI.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void OGLRenderAPI::Init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OGLRenderAPI::SetClearColor(Vec4 Color)
{
	glClearColor(Color.r, Color.g, Color.b, Color.a);
}

void OGLRenderAPI::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRenderAPI::SetViewport(const uint32_t& x, const uint32_t& y, const uint32_t& width, const uint32_t& height)
{
	glViewport(x, y, width, height);
}
