#pragma once

namespace WEngine
{

	class Layer;
	class Event;

	class LayerStack
	{
	public:

		LayerStack();

		~LayerStack();

		void PushLayer(Layer *pLayer);

		void OnEvent(Event *e);

		void OnUpdate();

	private:

		std::vector<Layer*> m_pLayers;

	};

}