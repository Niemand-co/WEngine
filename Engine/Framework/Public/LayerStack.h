#pragma once

namespace WEngine
{

	class Layer;

	class LayerStack
	{
	public:

		LayerStack();

		~LayerStack();

		void PushLayer(Layer *pLayer);

	private:

		std::vector<Layer*> m_pLayers;

	};

}