#include "WindowsRenderContext.h"
#include "Render/Framebuffer.h"
#include "Windows/WindowsWindow.h"

WindowsRenderContext::WindowsRenderContext(Window* window)
	: m_window(static_cast<WindowsWindow*>(window)), cur_buffer(0), m_bufferNum(2)
{
}

void WindowsRenderContext::Init()
{
	for (int i = 0; i < 2; ++i)
	{
		m_famebuffers[i] = new Framebuffer(m_window->GetWidth(), m_window->GetHeight());
		m_famebuffers[i]->SetBufferType(COLOR_BUFFER | DEPTH_BUFFER, 1);
	}
	Framebuffer::BindFramebuffer(m_famebuffers[0]);
}

void WindowsRenderContext::SetBufferNum(BUFFER_NUM num)
{
	switch (num)
	{
		case BUFFER_NUM::SINGLE_BUFFER:
		{
			m_bufferNum = 1;
		}
		case BUFFER_NUM::DOUBLE_BUFFER:
		{
			m_bufferNum = 2;
		}
		default:
		{
			std::cout << "ERROR TYPE!" << std::endl;
			exit(0);
		}
	}
}

void WindowsRenderContext::SwapBuffer()
{
	if (m_bufferNum != 2)
	{
		std::cout<<"ERROR NUM OF BUFFER"<<std::endl;
		exit(0);
	}
	m_famebuffers[cur_buffer]->RenderToScreen(m_window);
	cur_buffer = !cur_buffer;
	Framebuffer::BindFramebuffer(m_famebuffers[cur_buffer]);
}
