#include "pch.h"
#include "Framework/Public/Application.h"
#include "Utils/Public/Window.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"

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

	m_pipeline = ScriptableRenderPipeline::get();

	//m_pipeline->AddRenderer();

	//m_pipeline->Setup();

}

void Application::Tick()
{
	while (!IsQuit())
	{
		m_window->Update();
		if (m_window->GetShouldClose())
		{
			m_isQuit = true;
			m_window->Destroy();
			continue;
		}
		//m_pipeline->Execute();
	}
	Finalize();
}

bool Application::IsQuit()
{
	return m_isQuit;
}

void Application::Finalize()
{
	exit(0);
}
