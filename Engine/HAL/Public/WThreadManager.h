#pragma once
#include "HAL/Public/WThread.h"

namespace WEngine
{

	class WRunnable;

	class WThreadManager
	{
	public:

		WThreadManager() = default;

		~WThreadManager() = default;

		void AddThread(unsigned long threadID, WThread *pThread);

		//void* operator new(size_t size)
		//{
		//	return Allocator::Get()->Allocate(size);
		//}

		//void operator delete(void* pData)
		//{
		//	Allocator::Get()->Deallocate(pData);
		//}

	public:

		static WThreadManager* Get();

	private:

		static WThreadManager *g_instance;

	private:

		WHashMap<unsigned long, WThread*> m_threads;

	};

}