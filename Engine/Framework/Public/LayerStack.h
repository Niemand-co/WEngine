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

		const WArray<class Layer*>& GetLayers() const { return m_pLayers; }

		void* operator new(size_t size)
		{
			return Allocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			Allocator::Get()->Deallocate(pData);
		}

	public:

		static void Init() { g_instance = new LayerStack(); }

		static LayerStack* Get() { return g_instance; }

	private:

		static LayerStack *g_instance;

	private:

		WArray<Layer*> m_pLayers;

	};

}