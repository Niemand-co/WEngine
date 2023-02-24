#pragma once

namespace WEngine
{

	class WCriticalSectionWin
	{
	public:

		WCriticalSectionWin()
		{
			InitializeCriticalSection(&Section);
			SetCriticalSectionSpinCount(&Section, 4000);
		}

		~WCriticalSectionWin()
		{
			DeleteCriticalSection(&Section);
		}

		void Lock()
		{
			EnterCriticalSection(&Section);
		}

		bool TryLock()
		{
			if (TryEnterCriticalSection(&Section))
			{
				return true;
			}
			return false;
		}

		void Unlock()
		{
			LeaveCriticalSection(&Section);
		}

	private:

		CRITICAL_SECTION Section;

	};

}