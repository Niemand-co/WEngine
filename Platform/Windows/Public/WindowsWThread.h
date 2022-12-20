#pragma once
#include "HAL/Public/WThread.h"

namespace WEngine
{

	class WThreadWin : public WThread
	{
	public:

		WThreadWin();

		virtual ~WThreadWin();

		virtual bool IsValid() const override;

		virtual void Setup() override;

		virtual bool CreateInternal(WRunnable* pRunnable, const WString& name, size_t threadStackSize, WThread::ThreadPriority priority) override;

	private:

		HANDLE m_pThread;

	};

}