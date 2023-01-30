#include "pch.h"
#include "HAL/Public/TaskGraph.h"

namespace WEngine
{

	WTaskGraph* WTaskGraph::g_instance = nullptr;

	EThreadProperty GetThreadName(EThreadProperty property)
	{
		return EThreadProperty(EThreadProperty::NamedThreadNum & property);
	}

	EThreadProperty GetThreadPriority(EThreadProperty property)
	{
		return EThreadProperty(EThreadProperty::ThreadPriorityMask & property);
	}

	unsigned int GetQueueIndex(EThreadProperty property)
	{
		unsigned int index = (EThreadProperty::ThreadIndexMask & property);
		return index > 3 ? 3 : index;
	}

	WTaskGraph::WTaskGraph()
	{
		for (unsigned int threadIndex = 0; threadIndex < NUM_WORKING_THREAD; ++threadIndex)
		{
			m_threads[threadIndex] = (WThreadPack*)Allocator::Get()->Allocate(sizeof(WThreadPack));
			::new (m_threads[threadIndex]) WThreadPack();
			m_threads[threadIndex]->pTaskThread = new WNamedTaskThread();
			m_threads[threadIndex]->pTaskThread->m_threadId = (EThreadProperty)threadIndex;
		}
	}

	WTaskGraph::~WTaskGraph()
	{
	}

	void WTaskGraph::AttachToThread(EThreadProperty property)
	{
		EThreadProperty threadName = GetThreadName(property);
		m_threads[GetQueueIndex(property)]->bAttached = true;
	}

	void WTaskGraph::ProcessUntilQuit(EThreadProperty property)
	{
		unsigned int queueIndex = GetQueueIndex(property);
		property = GetThreadName(property);
		m_threads[queueIndex]->pTaskThread->ProcessUntilQuit();
	}

	void WTaskGraph::ProcessUntilIdle(EThreadProperty property)
	{
		unsigned int queueIndex = GetQueueIndex(property);
		property = GetThreadName(property);
		m_threads[queueIndex]->pTaskThread->ProcessUntilIdle();
	}

	void WTaskGraph::EnqueTask(WGraphTaskBase* task, EThreadProperty property)
	{
		m_threads[GetQueueIndex(property)]->pTaskThread->m_taskQueue.TaskQueue.Push(task);
	}

	WTaskThreadBase::~WTaskThreadBase()
	{
	}

	WNamedTaskThread::WNamedTaskThread()
	{
	}

	WNamedTaskThread::~WNamedTaskThread()
	{
	}

	void WNamedTaskThread::ProcessUntilQuit()
	{
		while (!m_taskQueue.bQuitForReturn)
		{
			ProcessNamedTaskThread();
		}
	}

	void WNamedTaskThread::ProcessUntilIdle()
	{
	}

	void WNamedTaskThread::RequistForQuit()
	{
		
	}

	void WNamedTaskThread::ProcessNamedTaskThread()
	{
		while (!m_taskQueue.TaskQueue.Empty() && !m_taskQueue.bQuitForReturn && !m_taskQueue.bQuitForIdle)
		{
			WGraphTaskBase* task = m_taskQueue.TaskQueue.Pop();
			if(task->ExecuteTask(EThreadProperty::RenderThread))
				delete task;
		}
	}

}