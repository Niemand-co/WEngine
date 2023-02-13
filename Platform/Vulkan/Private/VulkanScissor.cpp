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
		NormalAllocator::Get()->Deallocate(m_pScissor);
	}

}