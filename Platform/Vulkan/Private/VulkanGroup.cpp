#include "pch.h"
#include "Platform/Vulkan/Public/VulkanGroup.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

namespace Vulkan
{

	VulkanGroup::VulkanGroup(VulkanDevice* pInDevice)
		: pDevice(pInDevice)
	{
	
	}

	VulkanGroup::~VulkanGroup()
	{
	}

}