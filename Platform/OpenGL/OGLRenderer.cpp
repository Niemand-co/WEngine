#include "OGLRenderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

OGLRenderer::OGLRenderer(Primitive type)
{
	m_type = type;
}

OGLRenderer::~OGLRenderer()
{
}

void OGLRenderer::DrawByIndices(Entity* entity, Light* light, Camera* camera)
{
}
