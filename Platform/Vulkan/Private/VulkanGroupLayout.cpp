#include "pch.h"
#include "Platform/Vulkan/Public/VulkanGroupLayout.h"

VulkanGroupLayout::VulkanGroupLayout(VkDescriptorSetLayout* pDescriptorSetLayout, unsigned int bindingCount)
	: m_pDescriptorSetLayout(pDescriptorSetLayout)
{
	RHIGroupLayout::bindingCount = bindingCount;
}

VulkanGroupLayout::~VulkanGroupLayout()
{
}

VkDescriptorSetLayout* VulkanGroupLayout::GetHandle()
{
	return m_pDescriptorSetLayout;
}
