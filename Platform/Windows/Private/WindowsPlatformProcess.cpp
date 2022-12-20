#include "pch.h"
#include "Platform/Windows/Public/WindowsPlatformProcess.h"
#include "Platform/Windows/Public/WindowsWThread.h"

namespace WEngine
{

	WThread* WindowsPlatformProcess::CreateThread()
	{
		return new WThreadWin();
	}

	unsigned long WindowsPlatformProcess::GetCurrentThreadId()
	{
		return GetCurrentThreadId();
	}

}