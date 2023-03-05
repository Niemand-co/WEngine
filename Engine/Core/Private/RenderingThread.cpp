#include "pch.h"
#include "Core/Public/RenderingThread.h"
#include "Core/Public/RHIThread.h"
#include "HAL/Public/TaskGraph.h"
#include "HAL/Public/WEvent.h"
#include "HAL/Public/WThread.h"

namespace WEngine
{

	WRenderingThread::WRenderingThread()
		: pMainThreadSyncEvent(PlatformProcess::CreateWEvent())
	{
	}

	WRenderingThread::~WRenderingThread()
	{
	}

	void WRenderingThread::Init()
	{
	}

	unsigned int WRenderingThread::Run()
	{
		RenderThreadMain();

		return 0;
	}

	void WRenderingThread::Stop()
	{
	}

	void WRenderingThread::Exit()
	{
	}

	void WRenderingThread::RenderThreadMain()
	{
		WTaskGraph::Get()->AttachToThread(EThreadProperty::RenderThread);

		pMainThreadSyncEvent->Trigger();

		m_pRHIRunnable = new WRHIThread();
		m_pRHIThread = WThread::Create(m_pRHIRunnable, "RHI Thread");
		m_pRHIRunnable->m_pRenderingThreadSyncEvent->Wait();

		WTaskGraph::Get()->ProcessUntilQuit(EThreadProperty::RenderThread);
	}

}