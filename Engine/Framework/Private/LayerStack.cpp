#include "pch.h"
#include "Framework/Public/LayerStack.h"
#include "Framework/Public/Layer.h"
#include "Event/Public/Event.h"

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
	}

	void LayerStack::OnEvent(Event *e)
	{
		for (unsigned int i = m_pLayers.size() - 1; i >= 0; --i)
		{
			m_pLayers[i]->OnEvent(e);
			if(e->m_handled)
				break;
		}
	}

	void LayerStack::OnUpdate()
	{
		for (unsigned int i = 0; i < m_pLayers.size(); ++i)
		{
			m_pLayers[i]->OnUpdate();
		}
	}

}