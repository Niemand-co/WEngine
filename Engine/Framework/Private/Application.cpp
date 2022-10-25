#include "pch.h"
#include "Framework/Public/Application.h"
#include "Utils/Public/Window.h"
#include "Event/Public/KeyEvent.h"
#include "FrameWork/Public/LayerStack.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Event/Public/WindowInput.h"

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
		WEngine::Allocator::Init(WEngine::Backend::Vulkan);


		WinProc proc = { "WEngine", 1920u, 1080u };
		m_window = Window::Get(&proc);

		m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		Input::Init();

		m_pLayerStack = new LayerStack();
	}

	void Application::Tick()
	{
		while (!IsQuit())
		{
			m_pLayerStack->OnUpdate(TimeStep::GetTimeStep());
			m_window->Update();
			m_renderPipeline->Execute();
		}
		Finalize();
	}

	void Application::OnEvent(Event* pEvent)
	{
		EventDispatcher dispatcher(pEvent);
		dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent* e)->bool
		{
			if (e->GetKeycode() == GLFW_KEY_ESCAPE)
			{
				this->m_isQuit = true;
				this->m_window->Destroy();
				return true;
			}
			else
			{
				return false;
			}
		});

		m_pLayerStack->OnEvent(pEvent);
	}

	bool Application::IsQuit()
	{
		return m_isQuit;
	}

	void Application::Finalize()
	{
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