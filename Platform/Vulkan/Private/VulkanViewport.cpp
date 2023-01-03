#include "pch.h"
#include "Platform/Vulkan/Public/VulkanViewport.h"

namespace Vulkan
{

	VulkanViewport::VulkanViewport(VkViewport *pViewport)
		: m_pViewport(pViewport)
	{
		
	}

	VulkanViewport::~VulkanViewport()
	{
		WEngine::Allocator::Get()->Deallocate(m_pViewport);
	}

}