#include "pch.h"
#include "Framework/Public/Application.h"
#include "Utils/Public/Window.h"
#include "Scene/Public/World.h"
#include "Event/Public/KeyEvent.h"
#include "FrameWork/Public/LayerStack.h"

namespace WEngine
{

	Application* Application::m_instance = nullptr;

	Application::Application()
	{
		if(m_instance != nullptr)return;
		m_instance = this;
	}

	Application::~Application()
	{

	}

	Application* Application::CreateApplication()
	{
		if (m_instance == nullptr)
		{
			return new Application();
		}
		else
		{
			std::cout<<"Over Created Application"<<std::endl;
			exit(0);
		}
	}

	void Application::Init()
	{
		WEngine::Allocator::Init(WEngine::Backend::Vulkan);


		WinProc proc = { "WEngine", 1920u, 1080u };
		m_window = Window::Get(&proc);

		m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		World::CreateWorld();

	}

	void Application::Tick()
	{
		while (!IsQuit())
		{
			m_pLayerStack->OnUpdate();
			m_window->Update();
			if(m_window->GetIsClosed())
			{
				m_isQuit = true;
				m_window->Destroy();
				continue;
			}
		}
		Finalize();
	}

	void Application::OnEvent(Event* pEvent)
	{
		EventDispatcher dispatcher(pEvent);
		dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent*)->bool
		{
			this->m_isQuit = true;
			this->m_window->Destroy();
			return true;
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

}