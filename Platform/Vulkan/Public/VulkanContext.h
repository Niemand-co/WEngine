#pragma once
#include "RHI/Public/RHIContext.h"

namespace Vulkan
{

	class VulkanContext : public RHIContext
	{
	public:

		VulkanContext();

		virtual ~VulkanContext();

	};

}