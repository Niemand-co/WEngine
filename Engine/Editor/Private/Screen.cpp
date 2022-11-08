#include "pch.h"
#include "Editor/Public/Screen.h"

namespace WEngine
{

	unsigned int Screen::m_width = 960;

	unsigned int Screen::m_height = 720;

	Screen::Screen()
	{
	}

	void Screen::SetWidth(unsigned int width)
	{
		m_width = width;
	}

	void Screen::SetHeight(unsigned int height)
	{
		m_height = height;
	}

	unsigned int Screen::GetWidth()
	{
		return m_width;
	}

	unsigned int Screen::GetHeight()
	{
		return m_height;
	}

}