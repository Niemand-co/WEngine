#pragma once
#include "HAL/Public/Platform.h"

enum WRWLockType
{
	RW_Read = 0,
	RW_Write,
};

namespace WEngine
{

	class WRWScopeLock
	{
	public:

		WRWScopeLock(WRWLock* InLock, WRWLockType InType)
			: Lock(InLock),
			  Type(InType)
		{
			if (Type == RW_Read)
			{
				Lock->ReadLock();
			}
			else
			{
				Lock->WriteLock();
			}
		}

		~WRWScopeLock()
		{
			if (Type == RW_Read)
			{
				Lock->ReadUnlock();
			}
			else
			{
				Lock->WriteUnlock();
			}
		}

	private:

		WRWLock *Lock;

		WRWLockType Type;

	};

}