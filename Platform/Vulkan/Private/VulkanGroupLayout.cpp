#include "pch.h"
#include "Platform/Vulkan/Public/VulkanGroupLayout.h"

VulkanGroupLayout::VulkanGroupLayout(VkDescriptorSetLayout* pDescriptorSetLayout, unsigned int bindingCount)
	: m_pDescriptorSetLayout(pDescriptorSetLayout)
{
}

VulkanGroupLayout::~VulkanGroupLayout()
{
}

VkDescriptorSetLayout* VulkanGroupLayout::GetHandle()
{
	return m_pDescriptorSetLayout;
}
