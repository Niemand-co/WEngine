#include "pch.h"
#include "HAL/Public/TaskGraph.h"
#include "Core/Public/AnyThread.h"
#include "HAL/Public/WThread.h"
#include "HAL/Public/Platform.h"

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
		EThreadProperty index = (EThreadProperty)(property & EThreadProperty::ThreadIndexMask);
		switch (index)
		{
		case WEngine::GameThread:
			return 0;
		case WEngine::RenderThread:
			return 1;
		case WEngine::RHIThread:
			return 2;
		default:
			return index - 5;
		}
	}

	WTaskGraph::WTaskGraph()
	{
		for (unsigned int threadIndex = 0; threadIndex < NUM_WORKING_THREAD; ++threadIndex)
		{
			m_threads[threadIndex] = (WThreadPack*)GetCPUAllocator()->Allocate(sizeof(WThreadPack));
			::new (m_threads[threadIndex]) WThreadPack();
			m_threads[threadIndex]->pTaskThread = new WNamedTaskThread();
			m_threads[threadIndex]->pTaskThread->m_threadId = (EThreadProperty)threadIndex;
		}
		for (unsigned int threadIndex = EThreadProperty::AnyThread; threadIndex < EThreadProperty::AnyThread + 4; ++threadIndex)
		{
			WAnyThread *thread = new WAnyThread((EThreadProperty)threadIndex);
			WThread::Create(thread, "Rendering_Thread");
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

	WAnyTaskThread::WAnyTaskThread()
	{
		
	}

	WAnyTaskThread::~WAnyTaskThread()
	{
	}

	void WAnyTaskThread::ProcessUntilQuit()
	{
		while (!m_taskQueue.bQuitForReturn)
		{
			ProcessAnyTaskThread();
		}
	}

	void WAnyTaskThread::ProcessUntilIdle()
	{
	}

	void WAnyTaskThread::ProcessAnyTaskThread()
	{
		while (!m_taskQueue.TaskQueue.Empty() && !m_taskQueue.bQuitForReturn && !m_taskQueue.bQuitForIdle)
		{
			WGraphTaskBase* task = m_taskQueue.TaskQueue.Pop();
			if (task->ExecuteTask(EThreadProperty::RenderThread))
				delete task;
		}
	}

}

unsigned long GGameThreadId = 0;

unsigned long GRenderThreadId = 0;

unsigned long GRHIThreadId = 0;

bool IsRenderThread()
{
	return WEngine::PlatformProcess::GetThreadId() == GRenderThreadId;
}

bool IsOutOfRenderThread()
{
	return WEngine::PlatformProcess::GetThreadId() != GRenderThreadId;
}

bool IsRHIThread()
{
	return WEngine::PlatformProcess::GetThreadId() == GRHIThreadId;
}

bool IsOutOfRHIThread()
{
	return WEngine::PlatformProcess::GetThreadId() != GRHIThreadId;
}
