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

	void VulkanSurface::SetHandle(VkSurfaceKHR* surface)
	{
		m_surface = surface;
	}

	VkSurfaceKHR VulkanSurface::GetHandle()
	{
		return *m_surface;
	}

}
