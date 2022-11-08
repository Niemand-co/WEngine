#pragma once

class ScriptableRenderPipeline;

namespace WEngine
{

	class Event;

	class Layer
	{
	public:

		virtual void OnAttach() = 0;

		virtual void OnDettach() = 0;

		virtual void OnEvent(Event *pEvent) = 0;

		virtual void OnUpdate(TimeStep timeStep) = 0;

		virtual const std::string_view GetName() const
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

		Layer(std::string_view name);

		virtual ~Layer() = default;

	protected:

		std::string_view m_debugName;

		ScriptableRenderPipeline *m_pPipeline;

	};

}