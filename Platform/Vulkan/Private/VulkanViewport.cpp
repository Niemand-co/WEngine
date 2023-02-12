#include "pch.h"
#include "Platform/Vulkan/Public/VulkanViewport.h"

namespace Vulkan
{

	VulkanViewport::VulkanViewport(VulkanDevice *pInDevice)
		: pDevice(pInDevice)
	{
		
	}

	VulkanViewport::~VulkanViewport()
	{
		
	}

	void VulkanViewport::Tick()
	{
	}

	uint32 VulkanViewport::AcquireImageIndex()
	{
		return uint32();
	}

	void VulkanViewport::Present()
	{
	}

}