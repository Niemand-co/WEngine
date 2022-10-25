#include "pch.h"
#include "Event/Public/MouseButtonEvent.h"

namespace WEngine
{
	int MouseButtonEvent::GetMouseCode()
	{
		return m_mouseCode;
	}

	MouseButtonEvent::MouseButtonEvent(int mouseCode)
		: m_mouseCode(mouseCode)
	{
	}

	MouseButtonEvent::~MouseButtonEvent()
	{
	}

	MouseButtonPressedEvent::MouseButtonPressedEvent(int mouseCode)
		: MouseButtonEvent(mouseCode)
	{
	}

	MouseButtonPressedEvent::~MouseButtonPressedEvent()
	{
	}

	MouseButtonReleasedEvent::MouseButtonReleasedEvent(int mouseCode)
		: MouseButtonEvent(mouseCode)
	{
	}

	MouseButtonReleasedEvent::~MouseButtonReleasedEvent()
	{
	}

}