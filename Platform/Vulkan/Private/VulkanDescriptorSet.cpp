#include "pch.h"
#include "Platform/Vulkan/Public/VulkanDescriptorSet.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanShader.h"

namespace Vulkan
{

	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout()
	{
	}

	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
	}

	void VulkanDescriptorSetLayout::ProcessBindingForStage(VkShaderStageFlags ShaderStage, UniformBufferGatherInfo& OutGatherInfo, const VulkanShaderCodeHeader& CodeHeader)
	{
		for (const VulkanShaderCodeHeader::FUniformBufferInfo& UBInfo : CodeHeader.UniformBuffers)
		{
			OutGatherInfo.LayoutsToStageMap[UBInfo.LayoutHash] |= ShaderStage;
		}
		
		OutGatherInfo.CodeHeaders[(uint8)ShaderStage] = &CodeHeader;
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