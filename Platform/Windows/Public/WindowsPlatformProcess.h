#pragma once
#include "HAL/Public/PlatformProcess.h"
#include "Utils/Container/Public/WString.h"

namespace WEngine
{

	class WindowsPlatformProcess : public GenericPlatformProcess
	{
	public:

		static class WThread* CreateThread();

		static class WEvent* CreateWEvent();

		static unsigned long GetCurrentThreadId();

		template<typename T>
		static bool CAS(T* Destination, T NewValue, T CompareValue);

		template<typename T, typename X, typename Y>
		static bool CAS128(T* Destination, X HightValue, Y LowValue, T* CompareValue);

		template<typename LAMBDA>
		static void IterateFiles(const char* Path, LAMBDA lambda);
	};

#if defined(WINDOWS) || defined(_WIN32)
	typedef class WindowsPlatformProcess PlatformProcess;
#endif

	template<typename T>
	inline bool WindowsPlatformProcess::CAS(T* Destination, T NewValue, T CompareValue)
	{
		return InterlockedCompareExchange((volatile unsigned long long*)Destination, (unsigned long long)NewValue, (unsigned long long)CompareValue) == (unsigned long long)CompareValue;
	}

	template<typename T, typename X, typename Y>
	inline bool WindowsPlatformProcess::CAS128(T* Destination, X HightValue, Y LowValue, T* CompareValue)
	{
		return InterlockedCompareExchange128((volatile long long*)Destination, (long long)HightValue, (long long)LowValue, (long long*)CompareValue);
	}

	template<typename LAMBDA>
	inline void WindowsPlatformProcess::IterateFiles(const char* Path, LAMBDA lambda)
	{
		_finddata_t file;
		intptr_t handle;
		handle = _findfirst(Path, &file);
		if(handle == -1)
			return;
		lambda(file.name);
		while (!_findnext(handle, &file))
		{
			lambda(file.name);
		}
		_findclose(handle);
	}

}