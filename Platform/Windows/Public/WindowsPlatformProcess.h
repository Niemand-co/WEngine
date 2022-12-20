#pragma once
#include "HAL/Public/PlatformProcess.h"

namespace WEngine
{

	class WindowsPlatformProcess : public GenericPlatformProcess
	{
	public:

		static class WThread* CreateThread();

		static unsigned long GetCurrentThreadId();
	};

#if defined(WINDOWS) || defined(_WIN32)
	typedef class WindowsPlatformProcess PlatformProcess;
#endif

}