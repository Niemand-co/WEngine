#pragma once

namespace WEngine
{

	class GenericPlatformProcess
	{
	public:

		static class WThread* CreateThread() {}

		static class WEvent* CreateWEvent() {}

		static unsigned long GetCurrentThreadId() {}

	};

}