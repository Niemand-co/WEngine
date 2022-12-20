#include "pch.h"
#include "Platform/Windows/Public/WindowsWThread.h"
#include "HAL/Public/WThreadManager.h"
#include <windows.h>

namespace WEngine
{

	DWORD  __stdcall ThreadProc(LPVOID param)
	{
		WThreadWin *ThisThread = (WThreadWin*)param;
		WThreadManager::Get()->AddThread(ThisThread->GetPID(), ThisThread);
		return ThisThread->Run();
	}
	
	WThreadWin::WThreadWin()
	{
		m_pRunnable = nullptr;
	}

	WThreadWin::~WThreadWin()
	{
	}

	bool WThreadWin::IsValid() const
	{
		return m_pThread != NULL;
	}

	void WThreadWin::Setup()
	{
	}

	bool WThreadWin::CreateInternal(WRunnable* pRunnable, const WString& name, size_t threadStackSize, WThread::ThreadPriority priority)
	{
		m_pRunnable = pRunnable;
		m_name = name;
		m_threadStackSize = threadStackSize;
		m_priority = priority;

		m_pThread = CreateThread(NULL, m_threadStackSize, ThreadProc, this, STACK_SIZE_PARAM_IS_A_RESERVATION | CREATE_SUSPENDED, (LPDWORD)&m_id);

		if (m_pThread != NULL)
		{
			ResumeThread(m_pThread);
		}

		return m_pThread != NULL;
	}

}