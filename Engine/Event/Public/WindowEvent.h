#pragma once
#include "Event/Public/Event.h"

namespace WEngine
{

	class WindowEvent : public Event
	{
	public:

		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	protected:

		WindowEvent();

		virtual ~WindowEvent();

	};

	class WindowResizeEvent : public WindowEvent
	{
	public:

		WindowResizeEvent(unsigned int width, unsigned int height);

		virtual ~WindowResizeEvent();

		unsigned int GetWidth();

		unsigned int GetHeight();

		EVENT_CLASS_TYPE(WindowResize);

	private:

		unsigned int width;

		unsigned int height;

	};

	class WindowCloseEvent : public WindowEvent
	{
	public:

		WindowCloseEvent();

		virtual ~WindowCloseEvent();

		EVENT_CLASS_TYPE(WindowClose);

	};

}