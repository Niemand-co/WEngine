#pragma once

class Window;
class ScriptableRenderPipeline;

namespace WEngine
{

	class Event;
	class Layer;
	class LayerStack;

	class Application
	{
	public:

		Application();

		~Application();

		static Application* CreateApplication();

		void Init();

		void Tick();

		void OnEvent(Event *pEvent);

		bool IsQuit();

		void Finalize();

	private:

		static Application* m_instance;

	private:

		bool m_isQuit = false;

		Window* m_window;

		LayerStack *m_pLayerStack;

		ScriptableRenderPipeline *m_pipeline;

	};

}