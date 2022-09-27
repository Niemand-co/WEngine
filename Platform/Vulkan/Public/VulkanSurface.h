#pragma once
#include "RHI/Public/RHISurface.h"

namespace Vulkan
{

	class VulkanSurface : public RHISurface
	{
	public:

		VulkanSurface(VkSurfaceKHR *surface);

		virtual ~VulkanSurface();

		void SetHandle(VkSurfaceKHR *surface);

		VkSurfaceKHR GetHandle();

	private:

		VkSurfaceKHR *m_surface;

	};

}