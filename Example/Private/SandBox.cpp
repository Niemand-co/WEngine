#include "pch.h"
#include "Example/Public/SandBox.h"

GameScene::GameScene(std::string name)
	: Layer(name)
{

	m_isMoving = false;
	GameObject* go = World::GetWorld()->CreateGameObject("Camera");
	m_sceneCamera = go->AddComponent<Camera>();
	m_sceneCamera->SetRenderer(ScriptableRenderPipeline::get()->CreateRenderer());
	m_sceneCamera->m_aspect = (float)Window::cur_window->GetWidth() / (float)Window::cur_window->GetHeight();
	go->GetComponent<Transformer>()->SetPosition(glm::vec3(2.0f, 2.0f, 2.0f));
	
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
}

void GameScene::OnUpdate(WEngine::TimeStep timeStep)
{
	float OffsetX = WEngine::Input::GetMouseOffsetX();
	float OffsetY = WEngine::Input::GetMouseOffsetY();
	if (m_isMoving)
	{
		if (WEngine::Input::IsKeyPressed(GLFW_KEY_W))
		{
			m_sceneCamera->Move(Direction::FORWARD, (float)timeStep.GetSeconds() * 1.0f);
		}
		else if (WEngine::Input::IsKeyPressed(GLFW_KEY_S))
		{
			m_sceneCamera->Move(Direction::BACKWARD, (float)timeStep.GetSeconds() * 1.0f);
		}
		if (WEngine::Input::IsKeyPressed(GLFW_KEY_A))
		{
			m_sceneCamera->Move(Direction::LEFT, (float)timeStep.GetSeconds() * 1.0f);
		}
		else if (WEngine::Input::IsKeyPressed(GLFW_KEY_D))
		{
			m_sceneCamera->Move(Direction::RIGHT, (float)timeStep.GetSeconds() * 1.0f);
		}
		m_sceneCamera->Rotate(RotateDirection::Yaw,  OffsetX * 0.05f);
		m_sceneCamera->Rotate(RotateDirection::Pitch, OffsetY * 0.05f);
	}
}

GuiLayer::GuiLayer(std::string name)
	: Layer(name)
{

}

GuiLayer::~GuiLayer()
{
}

void GuiLayer::OnAttach()
{
}

void GuiLayer::OnDettach()
{
}

void GuiLayer::OnEvent(WEngine::Event* pEvent)
{
}

void GuiLayer::OnUpdate(WEngine::TimeStep timeStep)
{
}

SandBox::SandBox()
{
	m_scene = World::CreateWorld();

	m_renderPipeline = ScriptableRenderPipeline::get();

	GameObject *go = m_scene->CreateGameObject("Cube");
	MeshFilter *filter = go->AddComponent<MeshFilter>();
	filter->SetStaticMesh(Mesh::GetCube());
	go->AddComponent<Material>();
	Gui::SetSelectedObject(go);
	m_pLayerStack->PushLayer(new GameScene("Game Scene"));

	m_renderPipeline->Setup();
}

SandBox::~SandBox()
{
}


