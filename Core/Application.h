#pragma once
#include "utils/Window.h"
#include "Renderer/Rasterizer.h"

class Application
{
public:
	Application();
	~Application();

	void Init();
	void Tick();

private:
	static Application* m_instance;
	Window* m_window;
	Rasterizer* m_rasterizer;
};