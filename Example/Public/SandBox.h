#pragma once
#include "Sources/WEngine.h"

class GameScene : public WEngine::Layer
{
public:

	GameScene(const WEngine::WString& name);

	virtual ~GameScene();

	virtual void OnAttach() override;

	virtual void OnDettach() override;

	virtual void OnEvent(WEngine::Event* pEvent) override;

	virtual void Tick(WEngine::TimeStep timeStep) override;

private:
	
	bool m_isMoving;

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