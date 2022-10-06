#include "pch.h"
#include "Platform/Vulkan/Public/VulkanPipelineResourceLayout.h"

VulkanPipelineResourceLayout::VulkanPipelineResourceLayout(VkPipelineLayout* pPipelineLayout)
	: m_pPipelineLayout(pPipelineLayout)
{
}

VulkanPipelineResourceLayout::~VulkanPipelineResourceLayout()
{
}

VkPipelineLayout* VulkanPipelineResourceLayout::GetHandle()
{
	return m_pPipelineLayout;
}
