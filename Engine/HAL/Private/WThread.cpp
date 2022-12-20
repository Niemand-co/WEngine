#include "pch.h"
#include "HAL/Public/WThread.h"
#include "HAL/Public/WRunnable.h"
#include "HAL/Public/PlatformProcess.h"
#include "HAL/Public/Platform.h"

namespace WEngine
{

	WThread* WThread::Create(WRunnable* pRunnable, const WString& name, size_t threadStackSize, WThread::ThreadPriority priority)
	{
		WThread *thread = PlatformProcess::CreateThread();

		thread->CreateInternal(pRunnable, name, threadStackSize, priority);

		return thread;
	}

	unsigned int WThread::Run()
	{
		m_pRunnable->Init();

		unsigned int ExitCode = m_pRunnable->Run();

		m_pRunnable->Exit();

		return ExitCode;
	}

}