#pragma once
#include "Sources/WEngine.h"

class GameScene : public WEngine::Layer
{
public:

	GameScene(std::string name);

	virtual ~GameScene();

	virtual void OnAttach() override;

	virtual void OnDettach() override;

	virtual void OnEvent(WEngine::Event* pEvent) override;

	virtual void OnUpdate() override;

};

class SandBox : public WEngine::Application
{
public:

	SandBox();

	virtual ~SandBox();

};

WEngine::Application* WEngine::CreateApplication()
{
	return new SandBox();
}