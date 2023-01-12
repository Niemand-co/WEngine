#include "pch.h"
#include "Core/Public/Engine.h"
#include "Core/Public/RenderingThread.h"
#include "HAL/Public/WThread.h"

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
	}

	void GEngine::Init()
	{
	}

	void GEngine::Run()
	{
	}

	REngine::REngine()
	{
		PreInit();
	}

	REngine::~REngine()
	{
	}

	void REngine::PreInit()
	{
		InitRHIDevice();
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
	}

	void REngine::StartRenderingThread()
	{
		m_pRenderingRunnable = new WRenderingThread();
		m_pRenderingThread = WThread::Create(m_pRenderingRunnable, "Rendering_Thread");

	}

}