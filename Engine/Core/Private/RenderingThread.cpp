#include "pch.h"
#include "Core/Public/RenderingThread.h"
#include "HAL/Public/TaskGraph.h"
#include "HAL/Public/WEvent.h"

namespace WEngine
{

	WRenderingThread::WRenderingThread()
		: pMainThreadAsyncEvent(WEvent::Create())
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

		pMainThreadAsyncEvent->Trigger();

		WTaskGraph::Get()->ProcessUntilQuit(EThreadProperty::RenderThread);
	}

}