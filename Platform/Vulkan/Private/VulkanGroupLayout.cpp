#include "pch.h"
#include "Platform/Vulkan/Public/VulkanGroupLayout.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

namespace Vulkan
{

	VulkanGroupLayout::VulkanGroupLayout(VulkanDevice* pInDevice)
		: pDevice(pInDevice)
	{
		VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutCreateInfo = {};
		{
			DescriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			DescriptorSetLayoutCreateInfo.bindingCount = Bindings.Size();
			DescriptorSetLayoutCreateInfo.pBindings = Bindings.GetData();
		}
		vkCreateDescriptorSetLayout(pDevice->GetHandle(), &DescriptorSetLayoutCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &DescriptorSetLayout);
	}

	VulkanGroupLayout::~VulkanGroupLayout()
	{
	}

}