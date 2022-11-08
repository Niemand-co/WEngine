#include "pch.h"
#include "Utils/Public/Synchronizer.h"
#include "Platform/Vulkan/Public/VulkanSynchronizer.h"

namespace WEngine
{

	Synchronizer* Synchronizer::g_pSynchronizer = nullptr;

	std::vector<Trigger*> Synchronizer::g_pTriggers = std::vector<Trigger*>();

	void Synchronizer::Init(Backend backend)
	{
		if(g_pSynchronizer != nullptr)
			return;

		switch (backend)
		{
		case Synchronizer::Backend::None:
			break;
		case Synchronizer::Backend::Vulkan:
			g_pSynchronizer = new Vulkan::VulkanSynchronizer();
			return;
		default:
			break;
		}
	}

	Synchronizer* Synchronizer::Get()
	{
		return nullptr;
	}

	void Synchronizer::RegisterTrigger(Trigger* pTrigger)
	{
		g_pTriggers.push_back(pTrigger);
	}

	std::vector<Trigger*> Synchronizer::GetTrigger(std::string_view submissionName)
	{
		std::vector<Trigger*> triggers;
		for (Trigger* pTrigger : g_pTriggers)
		{
			if(pTrigger->IsNameIn(submissionName))
				triggers.push_back(pTrigger);
		}
		return triggers;
	}

}