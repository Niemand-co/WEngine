#include "pch.h"
#include "Event/Public/WindowEvent.h"

namespace WEngine
{

	WindowEvent::WindowEvent()
	{
	}

	WindowEvent::~WindowEvent()
	{
	}

	WindowResizeEvent::WindowResizeEvent(unsigned int width, unsigned int height)
		: width(width), height(height)
	{
	}

	WindowResizeEvent::~WindowResizeEvent()
	{
	}

	unsigned int WindowResizeEvent::GetWidth()
	{
		return width;
	}

	unsigned int WindowResizeEvent::GetHeight()
	{
		return height;
	}

	WindowCloseEvent::WindowCloseEvent()
	{
	}

	WindowCloseEvent::~WindowCloseEvent()
	{
	}

}