#include "pch.h"
#include "Platform/Vulkan/Public/VulkanGroupPool.h"
#include "Platform/Vulkan/Public/VulkanGroup.h"
#include "Platform/Vulkan/Public/VulkanGroupLayout.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Render/Descriptor/Public/RHIGroupLayoutDescriptor.h"

namespace Vulkan
{

	VulkanGroupPool::VulkanGroupPool(VulkanDevice *pInDevice, VkDescriptorPoolCreateInfo *pInfo, VkDescriptorSetLayout InDescriptorSetLayout)
		: pDevice(pInDevice)
	{
		vkCreateDescriptorPool(pDevice->GetHandle(), pInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &DescriptorSetPool);
	}

	VulkanGroupPool::~VulkanGroupPool()
	{
		vkDestroyDescriptorPool(pDevice->GetHandle(), DescriptorSetPool, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
		vkDestroyDescriptorSetLayout(pDevice->GetHandle(), DescriptorSetLayout, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	VkDescriptorSet VulkanGroupPool::GetDescriptorSet()
	{
		VkDescriptorSetAllocateInfo DescriptorSetAllocateInfo = {};
		{
			DescriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			DescriptorSetAllocateInfo.descriptorPool = DescriptorSetPool;
			DescriptorSetAllocateInfo.descriptorSetCount = 1;
			DescriptorSetAllocateInfo.pSetLayouts = &DescriptorSetLayout;
		}
		VkDescriptorSet DescriptorSet;
		vkAllocateDescriptorSets(pDevice->GetHandle(), &DescriptorSetAllocateInfo, &DescriptorSet);
		return DescriptorSet;
	}

}