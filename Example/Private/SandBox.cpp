#include "pch.h"
#include "Example/Public/SandBox.h"

GameScene::GameScene(const WEngine::WString& name)
	: Layer(name)
{
	Viewport = new WSceneViewport(1920, 1080, Format::B8G8R8A8_UNorm);
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
	Viewport->ProcessInput();
	
	WEngine::WTaskGraph::Get()->EnqueTask(new WEngine::WLambdaTask(true, [this]()
	{

		Scene->UpdateLightInfosForScene();

		Scene->UpdatePrimitiveInfosForScene();

		Scene->UpdateCameraInfoForScene();

		StartFrame();

		StartRendering();

		EndFrame();
		
	}), WEngine::EThreadProperty::RenderThread);
	
}

SandBox::SandBox()
{
	//GameObject *pLight = m_scene->CreateGameObject("Main Light");
	//pLight->AddComponent<LightComponent>();

	WEngine::LayerStack::Get()->PushLayer(new GameScene("Game Scene"));

	GameObject *sphere = GWorld::GetWorld()->CreateGameObject("Sphere");
	WStaticMesh *Mesh = WMeshLibrary::GetMesh("triangle");
	sphere->AddComponent<StaticMeshComponent>(Mesh);
	sphere->AddComponent<MaterialComponent>();
}

SandBox::~SandBox()
{
}


