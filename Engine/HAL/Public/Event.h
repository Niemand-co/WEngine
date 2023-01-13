#pragma once

namespace WEngine
{

	class WEvent
	{
	public:

		virtual ~WEvent() = default;

		virtual bool Create() = 0;

		virtual void Wait() = 0;

	};

}