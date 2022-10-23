#include "pch.h"
#include "Event/Public/Event.h"

namespace WEngine
{

	EventDispatcher::EventDispatcher(Event* pEvent)
		: m_pEvent(pEvent)
	{
		
	}

	EventDispatcher::~EventDispatcher()
	{
	}

}