#include "pch.h"
#include "Platform/Vulkan/Public/VulkanRenderPass.h"

namespace Vulkan
{

	VulkanRenderPass::VulkanRenderPass(VkRenderPass* renderPass)
		: m_renderPass(renderPass)
	{
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
	}

	VkRenderPass* VulkanRenderPass::GetHandle()
	{
		return m_renderPass;
	}

}