#include "pch.h"
#include "Utils/Public/Synchronizer.h"
#include "Platform/Vulkan/Public/VulkanSynchronizer.h"

namespace WEngine
{

	Synchronizer* Synchronizer::g_pSynchronizer = nullptr;

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

}