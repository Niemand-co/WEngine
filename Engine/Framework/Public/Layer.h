#pragma once

namespace WEngine
{

	class Event;

	class Layer
	{
	public:

		virtual ~Layer() = default;

		virtual void OnAttach() = 0;

		virtual void OnDettach() = 0;

		virtual void OnEvent(Event *pEvent) = 0;

		virtual void OnUpdate() = 0;

		virtual const std::string GetName() const
		{
			return m_debugName;
		}

	private:

		std::string m_debugName;

	};

}