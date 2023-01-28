#pragma once

class WViewport;

namespace WEngine
{

	class Event;

	class Layer
	{
	public:

		virtual void OnAttach() = 0;

		virtual void OnDettach() = 0;

		virtual void OnEvent(Event *pEvent) = 0;

		virtual void Tick(TimeStep timeStep) = 0;

		virtual const WString& GetName() const
		{
			return m_debugName;
		}

		void* operator new(size_t size)
		{
			return Allocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			Allocator::Get()->Deallocate(pData);
		}

	protected:

		Layer(const WString& name);

		virtual ~Layer() = default;

	protected:

		WString m_debugName;

		WViewport *m_pViewport;

	};

}