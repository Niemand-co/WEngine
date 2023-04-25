#include "pch.h"
#include "Platform/Vulkan/Public/VulkanDescriptorSet.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

namespace Vulkan
{

	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout()
	{
	}

	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
	}

	WEngine::WHashMap<uint32, VulkanDescriptorSetLayout*> VulkanDescriptorSetLayoutManager::Layouts = WEngine::WHashMap<uint32, VulkanDescriptorSetLayout*>();

	VulkanDescriptorSet::VulkanDescriptorSet(VulkanDevice* pInDevice)
		: pDevice(pInDevice)
	{
	
	}

	VulkanDescriptorSet::~VulkanDescriptorSet()
	{
	}

}