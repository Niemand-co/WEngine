#include "pch.h"
#include "Example/Public/SandBox.h"

GameScene::GameScene(std::string name)
	: Layer(name)
{
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
}

void GameScene::OnUpdate()
{
}

SandBox::SandBox()
{
	WEngine::Application();
	m_pLayerStack->PushLayer(new GameScene("Game Scene"));
}

SandBox::~SandBox()
{
}
