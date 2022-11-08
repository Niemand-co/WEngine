#include "pch.h"
#include "Framework/Public/LayerStack.h"
#include "Framework/Public/Layer.h"
#include "Event/Public/Event.h"
#include "RHI/Public/RHIContext.h"

namespace WEngine
{

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
	}

	void LayerStack::PushLayer(Layer* pLayer)
	{
		pLayer->OnAttach();
		if(m_pLayers.empty())
			m_pLayers.emplace_back(pLayer);
		else
			m_pLayers.emplace(m_pLayers.begin(), pLayer);
	}

	void LayerStack::PushOverLayer(Layer* pLayer)
	{
		m_pLayers.emplace_back(pLayer);
	}

	void LayerStack::OnEvent(Event *e)
	{
		for (int i = m_pLayers.size() - 1; i >= 0; --i)
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
		for (unsigned int i = 0; i < m_pLayers.size(); ++i)
		{
			m_pLayers[i]->OnUpdate(timeStep);
		}
	}

}