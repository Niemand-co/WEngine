#include "pch.h"
#include "Editor/Public/Screen.h"

namespace WEngine
{

	Screen::Screen()
	{
	}

	Screen::~Screen()
	{
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