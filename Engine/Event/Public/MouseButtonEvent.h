#pragma once
#include "Event/Public/Event.h"

namespace WEngine
{

	class MouseButtonEvent : public Event
	{
	public:

		int GetMouseCode();

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryMouse | EventCategoryInput);

	protected:

		MouseButtonEvent(int mouseCode);

		virtual ~MouseButtonEvent();

	private:

		int m_mouseCode;

	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:

		MouseButtonPressedEvent(int mouseCode);

		virtual ~MouseButtonPressedEvent();

		EVENT_CLASS_TYPE(MouseButtonPressed);

	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:

		MouseButtonReleasedEvent(int mouseCode);

		virtual ~MouseButtonReleasedEvent();

		EVENT_CLASS_TYPE(MouseButtonReleased);

	};

}