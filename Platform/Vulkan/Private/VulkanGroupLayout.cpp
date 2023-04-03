#include "pch.h"
#include "Platform/Vulkan/Public/VulkanGroupLayout.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

namespace Vulkan
{

	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(VulkanDevice* pInDevice, VkDescriptorSetLayoutCreateInfo* pInfo)
		: pDevice(pInDevice)
	{
		vkCreateDescriptorSetLayout(pDevice->GetHandle(), pInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &DescriptorSetLayout);
	}

	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(pDevice->GetHandle(), DescriptorSetLayout, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	WEngine::WHashMap<uint32, VulkanDescriptorSetLayout*> VulkanDescriptorSetLayoutManager::Layouts = WEngine::WHashMap<uint32, VulkanDescriptorSetLayout*>();

}