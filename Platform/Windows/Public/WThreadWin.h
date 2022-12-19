#pragma once
#include "HAL/Public/WThread.h"

namespace WEngine
{

	class WThreadWin : public WThread
	{
	public:

		WThreadWin(WRunnable* pRunnable, size_t threadStackSize, ThreadPriority priority);

		virtual ~WThreadWin();

		virtual void Setup() override;

		virtual void CreateInternal() override;

	private:

		HANDLE m_pThread;

	};

}