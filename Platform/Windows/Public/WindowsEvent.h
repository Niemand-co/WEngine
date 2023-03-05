#pragma once
#include "HAL/Public/WEvent.h"

namespace WEngine
{

	class WEventWin
	{
	public:

		WEventWin();

		virtual ~WEventWin();

		void Wait();

		void Reset();

		void Trigger();

		//void* operator new(size_t size);

		//void operator delete(void* pData);

	private:

		HANDLE m_event;

	};

}