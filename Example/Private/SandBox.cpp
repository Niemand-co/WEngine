#include "pch.h"
#include "Example/Public/SandBox.h"

GameScene::GameScene(const WEngine::WString& name)
	: Layer(name)
{
	m_isMoving = false;
	m_sceneCamera = WEngine::Editor::g_pEditorCamera;
	m_sceneCamera->m_aspect = (float)Window::cur_window->GetWidth() / (float)Window::cur_window->GetHeight();
	m_sceneCamera->GetOwner()->GetComponent<TransformComponent>()->SetPosition(glm::vec3(2.0f, 2.0f, 2.0f));
	WEngine::Screen::SetDisplayCamera(m_sceneCamera);
	
	m_pViewport = new WSceneViewport();
}

GameScene::~GameScene()
{
}

void GameScene::OnAttach()
{
}

void GameScene::OnDettach()
{
}

void GameScene::OnEvent(WEngine::Event* pEvent)
{
	WEngine::EventDispatcher dispatcher(pEvent);

	dispatcher.Dispatch<WEngine::MouseButtonPressedEvent>([this](WEngine::MouseButtonPressedEvent *pEvent) -> bool
	{
		if(pEvent->GetMouseCode() == GLFW_MOUSE_BUTTON_2)
			this->m_isMoving = true;
		return true;
	});

	dispatcher.Dispatch<WEngine::MouseButtonReleasedEvent>([this](WEngine::MouseButtonReleasedEvent* pEvent) -> bool
	{
		if(pEvent->GetMouseCode() == GLFW_MOUSE_BUTTON_2)
			this->m_isMoving = false;
		return true;
	});

	dispatcher.Dispatch<WEngine::WindowResizeEvent>([this](WEngine::WindowResizeEvent *pEvent)
	{
		
		return true;
	});
}

void GameScene::Tick(WEngine::TimeStep timeStep)
{
	m_pViewport->ProcessInput();
	
	WEngine::WTaskGraph::Get()->EnqueTask(new WEngine::WLambdaTask([]()
	{
		RScene::GetActiveScene()->UpdatePrimitiveInfosForScene();
	}
	), WEngine::EThreadProperty::RenderThread);
	
	GameObject::Find("Main Light")->GetComponent<LightComponent>()->UpdateShadowFrustum(m_sceneCamera->GetData());
}

SandBox::SandBox()
{
	//GameObject *pLight = m_scene->CreateGameObject("Main Light");
	//pLight->AddComponent<LightComponent>();

	//GameObject *plane = m_scene->CreateGameObject("Plane");
	//plane->AddComponent<MeshFilter>()->SetStaticMesh(StaticMesh::GetPlane());
	//plane->AddComponent<MaterialComponent>();

	//GameObject* go = m_scene->CreateGameObject("Cube");
	//MeshFilter* filter = go->AddComponent<MeshFilter>();
	//filter->SetStaticMesh(StaticMesh::GetCube());
	//go->AddComponent<MaterialComponent>();
	//plane->GetComponent<TransformComponent>()->SetScale(glm::vec3(50.0f, 50.0f, 50.0f));

	//m_pLayerStack->PushLayer(new GameScene("Game Scene"));

}

SandBox::~SandBox()
{
}


