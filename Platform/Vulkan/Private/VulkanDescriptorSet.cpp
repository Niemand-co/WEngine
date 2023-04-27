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

	void VulkanDescriptorSetLayout::FinalizeBindings(const VulkanDevice* Device, const UniformBufferGatherInfo& GatherInfo, const WEngine::WArray<RHISamplerState*>& InnutableSamplers)
	{
		VkDescriptorSetLayoutBinding Binding;
		WEngine::Memzero(Binding);
		Binding.descriptorCount = 1;

		for (uint8 Stage = 0; Stage < (uint8)EShaderStage::Count; ++Stage)
		{
			if (const VulkanShaderCodeHeader* CodeHeader = GatherInfo.CodeHeaders[Stage])
			{
				Binding.stageFlags = WEngine::ToVulkan((EShaderStage)Stage);
				for (int32 Index = 0; Index < CodeHeader->UniformBuffers.Size(); ++Index)
				{
					Binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

					const VulkanShaderCodeHeader::FUniformBufferInfo& UBInfo = CodeHeader->UniformBuffers[Index];
					uint32 LayoutHash = UBInfo.LayoutHash;
					VkShaderStageFlags ShaderStage = GatherInfo.LayoutsToStageMap[LayoutHash];
					if (ShaderStage)
					{
						
					}
				}
			}
		}

		Hash = WEngine::MemCrc32(Layouts.GetData(), sizeof(SetLayout) * Layouts.Size());
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