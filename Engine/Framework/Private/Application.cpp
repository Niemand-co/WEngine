#include "pch.h"
#include "Framework/Public/Application.h"
#include "Utils/Public/Window.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include <time.h>

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

struct test
{
	test() { a = 10; }
	~test() {}
	int a;
};

void Application::Init()
{
	WEngine::Allocator::Init(WEngine::Backend::Vulkan);

	WinProc proc = { "WEngine", 1920u, 1080u };
	m_window = Window::Get(&proc);

	m_pipeline = ScriptableRenderPipeline::get();

	//m_pipeline->AddRenderer();

	//m_pipeline->Setup();

	//float start, end;
	//start = std::clock();
	//for (int i = 0; i < 100000; ++i)
	//{
	//	test *t = (test*)WEngine::Allocator::Get()->Allocate(sizeof(test));
	//	::new (t) test();
	//	t->~test();
	//	WEngine::Allocator::Get()->Deallocate(t, sizeof(test));
	//}
	//end = std::clock();

	//std::cout<<end - start<<std::endl;

	//start = std::clock();
	//for (int i = 0; i < 100000; ++i)
	//{
	//	test* t = new test();
	//	delete t;
	//}
	//end = std::clock();

	//std::cout << end - start << std::endl;

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
