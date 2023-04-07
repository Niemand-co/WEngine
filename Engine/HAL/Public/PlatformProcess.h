#pragma once

namespace WEngine
{
	class WEventWin;
}

typedef WEngine::WEventWin WEvent;

namespace WEngine
{

	class GenericPlatformProcess
	{
	public:

		static class WThread* CreateThread() {}

		static WEvent* CreateWEvent() {}

		template<typename T>
		static bool CAS(volatile T* Destination, T NewValue, T CompareValue) {}

		template<typename T, typename Y>
		static bool CAS128(volatile T* Destination, Y HightValue, Y LowValue, T* CompareValue) {}

		static unsigned long GetThreadId() {}

		template<typename LAMBDA>
		static void IterateFiles(const char* Path, LAMBDA lambda) {}

	};

}