#include "pch.h"
#include "Platform/Windows/Public/WThreadWin.h"
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
	
	WThreadWin::WThreadWin(WRunnable *pRunnable, size_t threadStackSize, ThreadPriority priority)
	{
		m_pRunnable = pRunnable;
		m_threadStackSize = threadStackSize;
		m_priority = priority;

		CreateInternal();

		ResumeThread(m_pThread);
	}

	WThreadWin::~WThreadWin()
	{
	}

	void WThreadWin::Setup()
	{
	}

	void WThreadWin::CreateInternal()
	{
		m_pThread = CreateThread(NULL, m_threadStackSize, ThreadProc, this, STACK_SIZE_PARAM_IS_A_RESERVATION | CREATE_SUSPENDED, (LPDWORD)&m_id);
	}

}