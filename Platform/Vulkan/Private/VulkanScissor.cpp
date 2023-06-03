#include "pch.h"
#include "Platform/Vulkan/Public/VulkanScissor.h"

namespace Vulkan
{

	VulkanScissor::VulkanScissor(VkRect2D* pScissor)
		: m_pScissor(pScissor)
	{
		
	}

	VulkanScissor::~VulkanScissor()
	{
		GetCPUAllocator()->Deallocate(m_pScissor);
	}

}