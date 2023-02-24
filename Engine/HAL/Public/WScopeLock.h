#pragma once
#include "HAL/Public/Platform.h"

namespace WEngine
{

	class WScopeLock
	{
	public:

		WScopeLock(WCriticalSection *InSection)
			: Section(InSection)
		{
			Section->Lock();
		}

		~WScopeLock()
		{
			Section->Unlock();
		}

		void Unlock()
		{
			Section->Unlock();
		}

	private:

		WCriticalSection *Section;

	};

}