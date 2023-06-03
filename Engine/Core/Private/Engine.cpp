#include "pch.h"
#include "Core/Public/Engine.h"
#include "Core/Public/RenderingThread.h"
#include "HAL/Public/WThread.h"
#include "HAL/Public/WEvent.h"
#include "RHI/Public/RHIContext.h"
#include "Event/Public/Input.h"
#include "HAL/Public/TaskGraph.h"
#include "Render/Public/Scene.h"
#include "Framework/Public/LayerStack.h"
#include "Framework/Public/SlateLayer.h"
#include "Event/Public/TimeStep.h"
#include "Scene/Public/World.h"
#include "Render/Public/Shader.h"
#include "Render/Mesh/Public/Mesh.h"

namespace WEngine
{

	GEngine* GEngine::g_instance = nullptr;

	GEngine::GEngine()
	{
		
	}

	GEngine::~GEngine()
	{
	}

	void GEngine::PreInit()
	{
		Input::Init();
		LayerStack::Init();
	}

	void GEngine::Init()
	{
		//LayerStack::Get()->PushLayer(new GuiLayer("Gui"));
	}

	void GEngine::Tick(const TimeStep& timeStep)
	{
		Input::Poll();
		GFrameGameThread++;
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
		InitRHIDevice();
	}

	void REngine::Init()
	{
		GGameThreadId = GetCurrentThreadId();
		ResourceLoading();
		StartRenderingThread();
		//LayerStack::Get()->PushLayer(new GuiLayer("Gui"));
	}

	void REngine::Tick(const TimeStep& timeStep)
	{
		GEngine::Get()->Tick(timeStep);

		GFrameRenderThread++;
		const WArray<Layer*>& layers = LayerStack::Get()->GetLayers();
		for (int32 LayerIndex = layers.Size() - 1; LayerIndex >= 0; LayerIndex--)
		{
			layers[LayerIndex]->Tick(timeStep);
		}

		FrameSync::Sync();
	}

	void REngine::InitRHIDevice()
	{
		
	}

	void REngine::ResourceLoading()
	{
		PlatformProcess::IterateFiles("assets/*.*", [](const WEngine::WString& name)
		{
			if(name == "." || name == "..")
				return;
			WEngine::WString ResourceName = "assets/" + name;
			if(WShaderLibrary::LoadShader(ResourceName))
				return;
			if(WMeshLibrary::LoadMesh(ResourceName))
				return;
		});
	}

	void REngine::StartRenderingThread()
	{
		m_pRenderingRunnable = new WRenderingThread();
		m_pRenderingThread = WThread::Create(m_pRenderingRunnable, "Rendering_Thread");
		GRenderThreadId = m_pRenderingThread->GetPID();
		m_pRenderingRunnable->pMainThreadSyncEvent->Wait();
	}

	WTriggerTask FrameSync::SyncEvent[2] = { WTriggerTask(false), WTriggerTask(false) };

	uint32 FrameSync::FrameIndex = 0;

	void FrameSync::Sync()
	{
		WTaskGraph::Get()->EnqueTask(&SyncEvent[FrameIndex], EThreadProperty::RenderThread);
		SyncEvent[FrameIndex].Wait();
		FrameIndex = (FrameIndex + 1) % 2;
	}

}