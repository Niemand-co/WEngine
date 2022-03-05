#include "RenderCommand.h"

#ifdef OPENGL
RenderAPI* RenderCommand::m_renderAPI = new WindowsRenderAPI();
#endif

void RenderCommand::Init()
{
	m_renderAPI->Init();
}

void RenderCommand::SetClearColor(Vec4 Color)
{
	m_renderAPI->SetClearColor(Color);
}

void RenderCommand::Clear()
{
	m_renderAPI->Clear();
}

void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	m_renderAPI->SetViewport(x, y, width, height);
}

void RenderCommand::BeginScene()
{
}

void RenderCommand::EndScene()
{
}
