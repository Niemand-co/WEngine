#include "pch.h"
#include "Platform/Vulkan/Public/VulkanSampler.h"

VulkanSampler::VulkanSampler(VkSampler* pSampler)
	: m_pSampler(pSampler)
{
}

VulkanSampler::~VulkanSampler()
{
}

VkSampler* VulkanSampler::GetHandle()
{
	return m_pSampler;
}
