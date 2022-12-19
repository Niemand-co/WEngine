#include "pch.h"
#include "HAL/Public/WThread.h"
#include "HAL/Public/WRunnable.h"
#include "Platform/Windows/Public/WThreadWin.h"

namespace WEngine
{

	WThread* WThread::Create(WRunnable* pRunnable, const WString& name, size_t threadStackSize, WThread::ThreadPriority priority)
	{
#if defined(WINDOWS) || defined(_WIN) || defined(_WIN32)
		return new WThreadWin(pRunnable, name, threadStackSize, priority);
#else
		return nullptr;
#endif
	}

	unsigned int WThread::Run()
	{
		m_pRunnable->Init();

		unsigned int ExitCode = m_pRunnable->Run();

		m_pRunnable->Exit();

		return ExitCode;
	}

}