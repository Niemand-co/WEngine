#include "pch.h"
#include "HAL/Public/WThread.h"
#include "HAL/Public/WRunnable.h"

namespace WEngine
{

	unsigned int WThread::Run()
	{
		m_pRunnable->Init();

		unsigned int ExitCode = m_pRunnable->Run();

		m_pRunnable->Exit();

		return ExitCode;
	}

}