#pragma once
#include "RHI/Public/RHISurface.h"

namespace Vulkan
{

	class VulkanSurface : public RHISurface
	{
	public:

		VulkanSurface(VkSurfaceKHR *surface);

		virtual ~VulkanSurface();

		VkSurfaceKHR GetSurface();

	private:

		VkSurfaceKHR *m_surface;

	};

}