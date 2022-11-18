#include "pch.h"
#include "Platform/Vulkan/Public/VulkanGroupPool.h"
#include "Platform/Vulkan/Public/VulkanGroup.h"
#include "Platform/Vulkan/Public/VulkanGroupLayout.h"
#include "Render/Descriptor/Public/RHIGroupLayoutDescriptor.h"

VulkanGroupPool::VulkanGroupPool(VkDescriptorPool* pDescriptorSetPool, RHIGroupLayout* pGroupLayout, VkDevice *pDevice)
	: m_pDescriptorSetPool(pDescriptorSetPool), m_pDevice(pDevice)
{
	m_pGroupLayout = pGroupLayout;
}

VulkanGroupPool::~VulkanGroupPool()
{
}

std::vector<RHIGroup*> VulkanGroupPool::GetGroup(unsigned int count)
{
	std::vector<VkDescriptorSetLayout> layouts(count, *static_cast<VulkanGroupLayout*>(m_pGroupLayout)->GetHandle());
	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
	{
		descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocateInfo.descriptorPool = *m_pDescriptorSetPool;
		descriptorSetAllocateInfo.descriptorSetCount = count;
		descriptorSetAllocateInfo.pSetLayouts = layouts.data();
	}
	VkDescriptorSet *pDescriptorSets = (VkDescriptorSet*)WEngine::Allocator::Get()->Allocate(count * sizeof(VkDescriptorSet));
	vkAllocateDescriptorSets(*m_pDevice, &descriptorSetAllocateInfo, pDescriptorSets);

	std::vector<RHIGroup*> groups(count);
	for (unsigned int i = 0; i < count; ++i)
	{
		groups[i] = (VulkanGroup*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanGroup));
		::new (groups[i]) VulkanGroup(pDescriptorSets + i);
	}

	return groups;
}
