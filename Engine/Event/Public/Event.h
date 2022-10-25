#pragma once

namespace WEngine
{
	
	enum class EventType
	{
		None = 0,
		WindowResize, WindowClose, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, 
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = 0x00000001,
		EventCategoryInput       = 0x00000002,
		EventCategoryKeyboard    = 0x00000004,
		EventCategoryMouse       = 0x00000008,
		EventCategoryMouseButton = 0x00000010,
	};

	class Event
	{
	public:

		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;

		virtual const char* GetName() const = 0;
		
		virtual unsigned int GetCategoryFlags() const = 0;

		inline virtual bool IsInCategory(unsigned int category)
		{
			return (GetCategoryFlags() & category) == category;
		}

		void* operator new(size_t size)
		{
			return WEngine::Allocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			WEngine::Allocator::Get()->Deallocate(pData);
		}

	public:

		bool m_handled;

	};

	class EventDispatcher
	{
		template<typename type>
		using EventDipatchFunc = std::function<bool(type*)>;

	public:

		EventDispatcher(Event *pEvent);

		~EventDispatcher();

		template<typename type>
		void Dispatch(EventDipatchFunc<type> func)
		{
			if (m_pEvent->GetEventType() == type::GetStaticType())
			{
				m_pEvent->m_handled = func(static_cast<type*>(m_pEvent));
			}
		}

	private:
		
		Event *m_pEvent;

	};

}