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
		if(m_pLayers.Empty())
			m_pLayers.Push(pLayer);
		else
			m_pLayers.PushForward(pLayer);
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
		int currentImage = RHIContext::GetContext()->GetNextImage();
		if (currentImage < 0)
		{
			return;
		}
		for (unsigned int i = 0; i < m_pLayers.Size(); ++i)
		{
			m_pLayers[i]->Tick(timeStep);
		}
		RHIContext::GetContext()->Present(RHIContext::g_currentImage);
	}

}