#pragma once

namespace WEngine
{

	class Event;

	class Layer
	{
	public:

		virtual void OnAttach() = 0;

		virtual void OnDettach() = 0;

		virtual void OnEvent(Event *pEvent) = 0;

		virtual void OnUpdate() = 0;

		virtual const std::string GetName() const
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

		Layer(std::string name);

		virtual ~Layer() = default;

	private:

		std::string m_debugName;

	};

}