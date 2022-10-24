#include "pch.h"
#include "Event/Public/KeyEvent.h"

namespace WEngine
{

	KeyEvent::KeyEvent(int keycode)
		: m_keycode(keycode)
	{

	}

	int KeyEvent::GetKeycode()
	{
		return m_keycode;
	}

	KeyEvent::~KeyEvent()
	{
	}

	KeyPressedEvent::KeyPressedEvent(int keycode, int repeatCount)
		: KeyEvent(keycode), m_repeatCount(repeatCount)
	{
	}

	KeyPressedEvent::~KeyPressedEvent()
	{
	}

	KeyReleasedEvent::KeyReleasedEvent(int keycode)
		: KeyEvent(keycode)
	{
	}

	KeyReleasedEvent::~KeyReleasedEvent()
	{
	}

}