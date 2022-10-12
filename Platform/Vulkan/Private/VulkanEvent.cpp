#include "pch.h"
#include "Platform/Vulkan/Public/VulkanEvent.h"

VulkanEvent::VulkanEvent(VkEvent* pEvent)
	: m_pEvnet(pEvent)
{
}

VulkanEvent::~VulkanEvent()
{
}

VkEvent* VulkanEvent::GetHandle()
{
	return m_pEvnet;
}
