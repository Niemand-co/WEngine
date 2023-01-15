#include "pch.h"
#include "Platform/Windows/Public/WindowsPlatformProcess.h"
#include "Platform/Windows/Public/WindowsWThread.h"
#include "Platform/Windows/Public/WindowsEvent.h"

namespace WEngine
{

	WThread* WindowsPlatformProcess::CreateThread()
	{
		return new WThreadWin();
	}

	WEvent* WindowsPlatformProcess::CreateWEvent()
	{
		return new WEventWin();
	}

	unsigned long WindowsPlatformProcess::GetCurrentThreadId()
	{
		return GetCurrentThreadId();
	}

}