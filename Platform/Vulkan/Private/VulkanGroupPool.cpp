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

RHIGroup* VulkanGroupPool::GetGroup(unsigned int count)
{
	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
	{
		descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocateInfo.descriptorPool = *m_pDescriptorSetPool;
		descriptorSetAllocateInfo.descriptorSetCount = 1;
		descriptorSetAllocateInfo.pSetLayouts = static_cast<VulkanGroupLayout*>(m_pGroupLayout)->GetHandle();
	}

	VulkanGroup *group = (VulkanGroup*)WEngine::Allocator::Get()->Allocate(count * sizeof(VulkanGroup));
	for (unsigned int i = 0; i < count; ++i)
	{
		VkDescriptorSet *pDescriptorSets = (VkDescriptorSet*)WEngine::Allocator::Get()->Allocate(sizeof(VkDescriptorSet));
		vkAllocateDescriptorSets(*m_pDevice, &descriptorSetAllocateInfo, pDescriptorSets);

		::new (group + i) VulkanGroup(pDescriptorSets);
	}

	return group;
}
