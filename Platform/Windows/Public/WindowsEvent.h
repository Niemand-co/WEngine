#pragma once
#include "HAL/Public/WEvent.h"

namespace WEngine
{

	class WEventWin : public WEvent
	{
	public:

		WEventWin();

		virtual ~WEventWin();

		virtual void Wait() override;

		virtual void Trigger() override;

	private:

		HANDLE m_event;

	};

}