#include "pch.h"
#include "Core/Public/RHIThread.h"
#include "HAL/Public/TaskGraph.h"
#include "HAL/Public/WEvent.h"

namespace WEngine
{

	WRHIThread::WRHIThread()
		: m_pRenderingThreadSyncEvent(PlatformProcess::CreateWEvent())
	{
	}

	WRHIThread::~WRHIThread()
	{
	}

	void WRHIThread::Init()
	{
	}

	unsigned int WRHIThread::Run()
	{
		RHIThreadMain();
		return 0;
	}

	void WRHIThread::Stop()
	{
	}

	void WRHIThread::Exit()
	{
	}

	void WRHIThread::RHIThreadMain()
	{
		WTaskGraph::Get()->AttachToThread(EThreadProperty::RHIThread);

		m_pRenderingThreadSyncEvent->Trigger();

		WTaskGraph::Get()->ProcessUntilQuit(EThreadProperty::RHIThread);
	}

}