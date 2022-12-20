#pragma once

namespace WEngine
{

	class GenericPlatformProcess
	{
	public:

		static class WThread* CreateThread() {}

		static unsigned long GetCurrentThreadId() {}

	};

}