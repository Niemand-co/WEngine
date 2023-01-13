#pragma once
#include "HAL/Public/Event.h"

namespace WEngine
{

	class WEventWin : public WEvent
	{
	public:

		WEventWin();

		virtual ~WEventWin();

		virtual bool Create() override;

		virtual void Wait() override;

	private:

		HANDLE m_event;

	};

}