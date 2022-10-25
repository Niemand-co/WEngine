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

		virtual ~Application();

		virtual void Init();

		virtual void Tick();

		virtual void OnEvent(Event *pEvent);

		virtual bool IsQuit();

		virtual void Finalize();

		virtual void PushLayer(Layer *pLayer);

		virtual void PushOverLayer(Layer *pLayer);

	private:

		static Application* m_instance;

	protected:

		bool m_isQuit = false;

		Window* m_window;

		LayerStack *m_pLayerStack;

		ScriptableRenderPipeline* m_renderPipeline;

	};

	Application* CreateApplication();

}