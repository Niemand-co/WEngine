#pragma once
#include "Utils/Public/Synchronizer.h"

namespace Vulkan
{

	class VulkanSynchronizer : public WEngine::Synchronizer
	{
	public:

		VulkanSynchronizer();

		virtual ~VulkanSynchronizer();

		virtual void SetEvent(unsigned int index) override;

		virtual void WaitEvent(unsigned int index) override;

	private:

		std::unordered_map<unsigned int, VkEvent*> m_events;

	};

}