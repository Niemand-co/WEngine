#include "pch.h"
#include "Platform/Vulkan/Public/VulkanGroup.h"

VulkanGroup::VulkanGroup(VkDescriptorSet* pDescriptorSet)
	: m_pDescriptorSet(pDescriptorSet)
{
}

VulkanGroup::~VulkanGroup()
{
}

VkDescriptorSet* VulkanGroup::GetHandle()
{
	return m_pDescriptorSet;
}
