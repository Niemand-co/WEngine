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

	virtual void OnUpdate(WEngine::TimeStep timeStep) override;

private:
	
	bool m_isMoving;

	Camera *m_sceneCamera;

};

class GuiLayer : public WEngine::Layer
{
public:

	GuiLayer(std::string name);

	virtual ~GuiLayer();

	virtual void OnAttach() override;

	virtual void OnDettach() override;

	virtual void OnEvent(WEngine::Event *pEvent) override;

	virtual void OnUpdate(WEngine::TimeStep timeStep) override;

};

class SandBox : public WEngine::Application
{
public:

	SandBox();

	virtual ~SandBox();

private:

	World *m_scene;

};

WEngine::Application* WEngine::CreateApplication()
{
	return new SandBox();
}