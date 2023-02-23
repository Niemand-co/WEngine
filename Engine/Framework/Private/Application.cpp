#include "pch.h"
#include "Framework/Public/Application.h"
#include "Utils/Public/Window.h"
#include "Event/Public/KeyEvent.h"
#include "Event/Public/WindowEvent.h"
#include "FrameWork/Public/LayerStack.h"
#include "Event/Public/WindowInput.h"
#include "Framework/Public/SlateLayer.h"
#include "RHI/Public/RHIContext.h"
#include "Scene/Public/World.h"
#include "Editor/Public/Editor.h"
#include "Editor/Public/Debug.h"
#include "Core/Public/Engine.h"

namespace WEngine
{

	Application* Application::m_instance = nullptr;

	Application::Application()
	{
		Init();
	}

	Application::~Application()
	{

	}

	void Application::Init()
	{
		//NormalAllocator::Init(WEngine::Backend::Vulkan);

		WinProc proc = { "WEngine", 1920u, 1080u };
		m_window = Window::Get(&proc);

		m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		//RHIContext::Init();

		//Input::Init();

		//World::CreateWorld();

		//Editor::Init();

		//Debug::Init();

		LayerStack::Init();
		m_pLayerStack = LayerStack::Get();

		//m_pLayerStack->PushLayer(new GuiLayer("ImGui"));

		GEngine::Get()->PreInit();
		REngine::Get()->PreInit();

		GEngine::Get()->Init();
		REngine::Get()->Init();
	}

	void Application::Tick()
	{
		while (!IsQuit())
		{
			m_window->Poll();
			TimeStep timeStep = TimeStep::GetTimeStep();
			REngine::Get()->Tick(timeStep);
		}
		Finalize();
	}

	void Application::OnEvent(Event* pEvent)
	{
	}

	bool Application::IsQuit()
	{
		return m_isQuit;
	}

	void Application::Finalize()
	{
		m_window->Destroy();
		exit(0);
	}

	void Application::PushLayer(Layer* pLayer)
	{
		m_pLayerStack->PushLayer(pLayer);
	}

	void Application::PushOverLayer(Layer* pLayer)
	{
		m_pLayerStack->PushOverLayer(pLayer);
	}

}