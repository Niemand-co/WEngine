#include "pch.h"
#include "Example/Public/SandBox.h"

GameScene::GameScene(const WEngine::WString& name)
	: Layer(name)
{
	m_pViewport = new WSceneViewport(1920, 1080, Format::A16R16G16B16_SFloat);
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
	
	RScene& scene = *RScene::GetActiveScene();

	WEngine::WTaskGraph::Get()->EnqueTask(new WEngine::WLambdaTask(true, [&scene]()
	{
		scene.UpdateLightInfosForScene();

		scene.UpdatePrimitiveInfosForScene();

		scene.UpdateCameraInfoForScene();
	}
	), WEngine::EThreadProperty::RenderThread);

	WEngine::WTaskGraph::Get()->EnqueTask(new WEngine::WLambdaTask(true, [&scene]()
	{
		scene.StartFrame();
	}
	), WEngine::EThreadProperty::RenderThread);
	
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

	WEngine::LayerStack::Get()->PushLayer(new GameScene("Game Scene"));
	RScene::SetActiveScene(new RScene);

	GameObject *go = GWorld::GetWorld()->CreateGameObject("Camera");
	CameraComponent *camera = go->AddComponent<CameraComponent>();
	camera->SetRenderer<DeferredRenderer>();
}

SandBox::~SandBox()
{
}


