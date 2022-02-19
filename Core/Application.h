#pragma once
#include "Render/Rasterizer.h"
#include "Scene/World.h"
#include "Render/Framebuffer.h"

class Application
{
public:
	Application();
	~Application();

	static Application* CreateApplication();

	void Init();
	void Tick();

private:
	static Application* m_instance;
	Window* m_window;
	World* m_world;
	std::vector<Framebuffer> m_framebuffers;
};