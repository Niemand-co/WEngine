#include "pch.h"
#include "Platform/Vulkan/Public/VulkanLayout.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanDescriptorSet.h"

namespace Vulkan
{

	VulkanLayout::VulkanLayout(VulkanDevice* pInDevice, const VulkanDescriptorSetLayout& DescriptorSetLayout)
		: pDevice(pInDevice)
	{
		VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo;
		ZeroVulkanStruct(PipelineLayoutCreateInfo, VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO);
		const WEngine::WArray<VulkanDescriptorSetLayout::SetLayout>& Layouts = DescriptorSetLayout.GetLayouts();
		PipelineLayoutCreateInfo.setLayoutCount = Layouts.Size();
		VkDescriptorSetLayout DescriptorSetLayouts[1];
		VkDescriptorSetLayoutCreateInfo;
		for (uint32 Index = 0; Index < Layouts.Size(); ++Index)
		{
			DescriptorSetLayouts[Index].
		}
	}

	VulkanLayout::~VulkanLayout()
	{
		vkDestroyPipelineLayout(pDevice->GetHandle(), PipelineLayout, static_cast<VulkanAllocator*>(GetCPUAllocator())->GetCallbacks());
	}

}