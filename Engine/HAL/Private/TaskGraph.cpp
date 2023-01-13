#include "pch.h"
#include "HAL/Public/TaskGraph.h"

namespace WEngine
{

	WTaskGraph* WTaskGraph::g_instance = nullptr;

	WTaskGraph::WTaskGraph()
	{
		for (unsigned int threadIndex = 0; threadIndex < NUM_WORKING_THREAD; ++threadIndex)
		{
			m_threads[threadIndex] = (WThreadPack*)Allocator::Get()->Allocate(sizeof(WThreadPack));
			::new (m_threads[threadIndex]) WThreadPack();
		}
	}

	WTaskGraph::~WTaskGraph()
	{
	}

	void WTaskGraph::AttachToThread(EThreadProperty property)
	{
		EThreadProperty threadName = GetThreadName(property);
		if (threadName < EThreadProperty::NamedThreadNum)
		{
			m_threads[threadName]->bAttached = true;
			m_threads[threadName]->pThread = 
		}
	}

}