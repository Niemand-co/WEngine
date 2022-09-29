#pragma once

class Window;
class ScriptableRenderPipeline;

class Application
{
public:
	Application();
	~Application();

	static Application* CreateApplication();

	void Init();

	void Tick();

	bool IsQuit();

	void Finalize();

private:
	static Application* m_instance;
private:
	bool m_isQuit = false;
	Window* m_window;
	ScriptableRenderPipeline *m_pipeline;
};