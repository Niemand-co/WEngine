#include "pch.h"
#include "Platform/Vulkan/Public/VulkanGroupPool.h"
#include "Platform/Vulkan/Public/VulkanGroup.h"
#include "Platform/Vulkan/Public/VulkanGroupLayout.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Render/Descriptor/Public/RHIGroupLayoutDescriptor.h"

namespace Vulkan
{

	VulkanGroupPool::VulkanGroupPool(VulkanDevice *pInDevice, VkDescriptorPoolCreateInfo *pInfo)
		: pDevice(pInDevice)
	{
		vkCreateDescriptorPool(pDevice->GetHandle(), pInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &DescriptorSetPool);
	}

	VulkanGroupPool::~VulkanGroupPool()
	{
		vkDestroyDescriptorPool(pDevice->GetHandle(), DescriptorSetPool, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	WEngine::WArray<RHIGroup*> VulkanGroupPool::GetGroup(uint32 count)
	{
		WEngine::WArray<VkDescriptorSetLayout> layouts(count, *static_cast<VulkanGroupLayout*>(m_pGroupLayout)->GetHandle());
		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
		{
			descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			descriptorSetAllocateInfo.descriptorPool = DescriptorSetPool;
			descriptorSetAllocateInfo.descriptorSetCount = count;
			descriptorSetAllocateInfo.pSetLayouts = layouts.GetData();
		}
		VkDescriptorSet *pDescriptorSets = (VkDescriptorSet*)NormalAllocator::Get()->Allocate(count * sizeof(VkDescriptorSet));
		vkAllocateDescriptorSets(pDevice->GetHandle(), &descriptorSetAllocateInfo, pDescriptorSets);

		WEngine::WArray<RHIGroup*> groups(count);
		for (unsigned int i = 0; i < count; ++i)
		{
			groups[i] = new VulkanGroup(pDevice);
		}

		return groups;
	}

}