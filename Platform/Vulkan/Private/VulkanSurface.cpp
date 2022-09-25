#include "pch.h"
#include "Platform/Vulkan/Public/VulkanSurface.h"

namespace Vulkan
{

	VulkanSurface::VulkanSurface(VkSurfaceKHR *surface)
		: m_surface(surface)
	{
	}

	VulkanSurface::~VulkanSurface()
	{
	}

	VkSurfaceKHR VulkanSurface::GetSurface()
	{
		return *m_surface;
	}

}
