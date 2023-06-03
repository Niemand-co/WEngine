#include "pch.h"
#include "Framework/Public/LayerStack.h"
#include "Framework/Public/Layer.h"
#include "Event/Public/Event.h"
#include "RHI/Public/RHIContext.h"

namespace WEngine
{

	LayerStack* LayerStack::g_instance = nullptr;

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
	}

	void LayerStack::PushLayer(Layer* pLayer)
	{
		pLayer->OnAttach();
		m_pLayers.Push(pLayer);
	}

	void LayerStack::PushOverLayer(Layer* pLayer)
	{
		m_pLayers.Push(pLayer);
	}

	void LayerStack::OnEvent(Event *e)
	{
		for (int i = m_pLayers.Size() - 1; i >= 0; --i)
		{
			m_pLayers[i]->OnEvent(e);
			if(e->m_handled)
				break;
		}
	}

	void LayerStack::OnUpdate(TimeStep timeStep)
	{
	}

}