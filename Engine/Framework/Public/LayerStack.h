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

		void PushOverLayer(Layer *pLayer);

		void OnEvent(Event *e);

		void OnUpdate(TimeStep timeStep);

		void* operator new(size_t size)
		{
			return Allocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			Allocator::Get()->Deallocate(pData);
		}

	private:

		std::vector<Layer*> m_pLayers;

	};

}