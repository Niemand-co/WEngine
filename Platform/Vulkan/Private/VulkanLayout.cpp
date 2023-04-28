#include "pch.h"
#include "Platform/Vulkan/Public/VulkanLayout.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanDescriptorSet.h"

namespace Vulkan
{

	VulkanLayout::VulkanLayout(VulkanDevice* pInDevice)
		: pDevice(pInDevice)
	{
		//WEngine::WArray<VkDescriptorSetLayout> SetLayouts(DescriptorSetLayouts.Size());
		//for (uint32 Index = 0; Index < SetLayouts.Size(); ++Index)
		//{
		//	SetLayouts[Index] = DescriptorSetLayouts[Index].GetHandle();
		//}
		//VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo = {};
		//{
		//	PipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		//	PipelineLayoutCreateInfo.setLayoutCount = SetLayouts.Size();
		//	PipelineLayoutCreateInfo.pSetLayouts = SetLayouts.GetData();
		//}
		//vkCreatePipelineLayout(pDevice->GetHandle(), &PipelineLayoutCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &PipelineLayout);
	}

	VulkanLayout::~VulkanLayout()
	{
		vkDestroyPipelineLayout(pDevice->GetHandle(), PipelineLayout, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

}