#pragma once

namespace WEngine
{

	class WRWLockWin
	{
	public:

		WRWLockWin(const WRWLockWin&) = delete;
		WRWLockWin& operator=(const WRWLockWin&) = delete;

		WRWLockWin()
		{
			InitializeSRWLock(&Lock);
		}

		void ReadLock()
		{
			AcquireSRWLockShared(&Lock);
		}

		void WriteLock()
		{
			AcquireSRWLockExclusive(&Lock);
		}

		void ReadUnlock()
		{
			ReleaseSRWLockShared(&Lock);
		}

		void WriteUnlock()
		{
			ReleaseSRWLockExclusive(&Lock);
		}

	private:

		SRWLOCK Lock;

	};

}