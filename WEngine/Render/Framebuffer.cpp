#include "Framebuffer.h"
#include "Platform/Windows/WindowsWindow.h"
#include <float.h>

Framebuffer* Framebuffer::cur_framebuffer = nullptr;
bool Framebuffer::isDepthTest = false;
Vec4 Framebuffer::clear_color = Vec4(0.0f, 0.0f, 0.0f, 1.0f);

BUFFER_TYPE COLOR_BUFFER = 0b00000001;
BUFFER_TYPE DEPTH_BUFFER = 0b00000010;

Framebuffer::Framebuffer(uint32_t width, uint32_t height)
	: m_width(width), m_height(height)
{
}

uint32_t Framebuffer::GetWidth()
{
	return m_width;
}

uint32_t Framebuffer::GetHeight()
{
	return m_height;
}

void Framebuffer::SetDepthTest(bool test)
{
	isDepthTest = test;
}

void Framebuffer::SetBufferType(BUFFER_TYPE type, uint8_t num)
{
	if((type & COLOR_BUFFER) == COLOR_BUFFER)
	{
		m_colorbuffers.resize(num);
		for (std::vector<Vec4>& buffer : m_colorbuffers)
		{
			buffer.resize(m_width * m_height);
		}
	}
	if((type & DEPTH_BUFFER) == DEPTH_BUFFER)
	{
		m_depthbuffer.resize((size_t)m_width * (size_t)m_height);
	}
}

void Framebuffer::WriteToBuffer(uint32_t x, uint32_t y, std::vector<Vec4> buffer)
{
	uint32_t coord = x + y * m_width;
	if(coord >= m_width * m_height)return;
	if (isDepthTest)
	{
		if (m_depthbuffer[coord].z < buffer[buffer.size() - 1].z)
		{
			for(uint32_t i = 0; i < m_colorbuffers.size(); ++i)
				m_colorbuffers[i][coord] = buffer[i];
			m_depthbuffer[coord] = buffer[buffer.size() - 1];
		}
	}
	else
	{
		for (uint32_t i = 0; i < m_colorbuffers.size(); ++i)
			m_colorbuffers[i][coord] = buffer[i];
	}
}

void Framebuffer::ClearBuffer(BUFFER_TYPE type)
{
	if ((type & COLOR_BUFFER) == COLOR_BUFFER)
	{
		for (std::vector<Vec4>& buffer : cur_framebuffer->m_colorbuffers)
		{
			for (uint32_t i = 0; i < buffer.size(); ++i)
			{
				buffer[i] = clear_color;
			}
		}
	}
	if ((type & DEPTH_BUFFER) == DEPTH_BUFFER && cur_framebuffer->m_depthbuffer.size() != 0)
	{
		for (uint32_t i = 0; i < cur_framebuffer->m_depthbuffer.size(); ++i)
		{
			cur_framebuffer->m_depthbuffer[i] = Vec4(-FLT_MAX);
		}
	}
}

void Framebuffer::RenderToScreen(WindowsWindow* window)
{
	for (uint32_t i = 0; i < m_colorbuffers[0].size(); ++i)
	{
		window->DrawPixel(i % m_width, i / m_width, m_colorbuffers[0][i]);
	}
}

void Framebuffer::BindFramebuffer(Framebuffer* framebuffer)
{
	cur_framebuffer = framebuffer;
}

void Framebuffer::SetClearColor(Vec4 color)
{
	clear_color = color;
}

FramebufferLibrary::FramebufferLibrary()
	: m_head(nullptr)
{
}

FramebufferLibrary::~FramebufferLibrary()
{
	Clear();
}

void FramebufferLibrary::Init()
{
	m_head = new StackAllocator();
}

Framebuffer* FramebufferLibrary::Allocate(uint32_t width, uint32_t height)
{
	return nullptr;
}

uint32_t FramebufferLibrary::GetMark()
{
	return m_mark;
}

void FramebufferLibrary::Clear()
{
}
