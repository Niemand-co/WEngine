#include "pch.h"
#include "Framework/Public/Application.h"
#include "Utils/Public/Window.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Scene/Public/GameObject.h"
#include "Scene/Components/Public/MeshFilter.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Scene/Public/World.h"

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

		World::CreateWorld();

		GameObject *go = World::GetWorld()->CreateGameObject("Cube");
		static_cast<Transformer*>(go->GetComponent<Component::ComponentType::Transformer>())->SetPosition(glm::vec3(2.0f, 2.0f, 2.0f));
		MeshFilter *filter = (MeshFilter*)go->AddComponent<Component::ComponentType::MeshFilter>();
		filter->SetStaticMesh(Mesh::GetCube());

		Camera *camera = (Camera*)go->AddComponent<Component::ComponentType::Camera>();
		camera->m_aspect = (float)Window::cur_window->GetWidth() / (float)Window::cur_window->GetHeight();

		GameObject *floor = World::GetWorld()->CreateGameObject("Floor");
		filter = (MeshFilter*)floor->AddComponent<Component::ComponentType::MeshFilter>();
		filter->SetStaticMesh(Mesh::GetPlane());

		go->AddComponent<Component::ComponentType::Material>();

		m_pipeline = ScriptableRenderPipeline::get();

		ScriptableRenderer *renderer = m_pipeline->CreateRenderer();

		camera->SetRenderer(renderer);

		m_pipeline->Setup();

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
			m_pipeline->Execute();
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
		exit(0);
	}

}