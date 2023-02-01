#pragma once
#include "RHI/Public/RHISurface.h"

namespace Vulkan
{

	class VulkanSurface
	{
	public:

		VulkanSurface();

		virtual ~VulkanSurface();

	private:

		VkImage* pImage;

		VkImageView* pImageView;

	};

}