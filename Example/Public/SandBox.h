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

	CameraComponent *m_sceneCamera;

};

class SandBox : public WEngine::Application
{
public:

	SandBox();

	virtual ~SandBox();

private:

	GWorld *m_scene;

};

WEngine::Application* WEngine::CreateApplication()
{
	return new SandBox();
}