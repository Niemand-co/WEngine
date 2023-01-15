#pragma once

namespace WEngine
{

	class WEvent
	{
	public:

		virtual ~WEvent() = default;

		virtual void Wait() = 0;

		virtual void Trigger() = 0;

	public:

		static WEvent* Create();

	};

}