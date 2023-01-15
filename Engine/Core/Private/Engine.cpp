#include "pch.h"
#include "Core/Public/Engine.h"
#include "Core/Public/RenderingThread.h"
#include "HAL/Public/WThread.h"
#include "HAL/Public/WEvent.h"
#include "RHI/Public/RHIContext.h"
#include "Event/Public/Input.h"

namespace WEngine
{

	GEngine::GEngine()
	{
		
	}

	GEngine::~GEngine()
	{
	}

	void GEngine::PreInit()
	{
		Input::Init();
	}

	void GEngine::Init()
	{
	}

	void GEngine::Run()
	{
	}

	REngine* REngine::g_instance = nullptr;

	REngine::REngine()
	{
	}

	REngine::~REngine()
	{
	}

	void REngine::PreInit()
	{
		//InitRHIDevice();
		StartRenderingThread();
	}

	void REngine::Init()
	{
	}

	void REngine::Run()
	{
	}

	void REngine::InitRHIDevice()
	{
		RHIContext::Init();
	}

	void REngine::StartRenderingThread()
	{
		m_pRenderingRunnable = new WRenderingThread();
		m_pRenderingThread = WThread::Create(m_pRenderingRunnable, "Rendering_Thread");
		m_pRenderingRunnable->pMainThreadAsyncEvent->Wait();
	}

}