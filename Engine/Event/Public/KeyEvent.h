#pragma once
#include "Event/Public/Event.h"

namespace WEngine
{

	class KeyEvent : public Event
	{
	public:

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);

	protected:

		KeyEvent(int keycode);

		virtual ~KeyEvent();

	private:

		int m_keycode;

	};

	class KeyPressedEvent : public KeyEvent
	{
	public:

		KeyPressedEvent(int keycode, int repeatCount);

		virtual ~KeyPressedEvent();

		EVENT_CLASS_TYPE(KeyPressed);

	private:

		int m_repeatCount;

	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:

		KeyReleasedEvent(int keycode);

		virtual ~KeyReleasedEvent();

		EVENT_CLASS_TYPE(KeyReleased);

	};

}