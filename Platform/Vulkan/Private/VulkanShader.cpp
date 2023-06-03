#include "pch.h"
#include "Platform/Vulkan/Public/VulkanShader.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Render/Descriptor/Public/RHIShaderDescriptor.h"
#include "Render/Public/RenderDependencyGraphParameter.h"

namespace Vulkan
{

	VulkanShader::VulkanShader(VulkanDevice *pInDevice, RHIShaderDescriptor* Descriptor)
		: RHIShader(Descriptor->Frequency),
		  pDevice(pInDevice)
	{
		VkShaderModuleCreateInfo ShaderModuleCreateInfo;
		ZeroVulkanStruct(ShaderModuleCreateInfo, VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO);
		ShaderModuleCreateInfo.codeSize = Descriptor->CodeSize;
		ShaderModuleCreateInfo.pCode = Descriptor->Code;

		RE_ASSERT(vkCreateShaderModule(pDevice->GetHandle(), &ShaderModuleCreateInfo, static_cast<VulkanAllocator*>(GetCPUAllocator())->GetCallbacks(), &ShaderModule) == VK_SUCCESS, "Failed to creat shader module");
	}

	VulkanShader::~VulkanShader()
	{
		vkDestroyShaderModule(pDevice->GetHandle(), ShaderModule, static_cast<VulkanAllocator*>(GetCPUAllocator())->GetCallbacks());
	}

	void VulkanShader::SetupParametersInternal(const ShaderParametersLayout& Layout)
	{
		const WEngine::WArray<ShaderParametersLayout::ResourceInfo>& UniformBuffers =  Layout.GetUniforms();

		for (int32 Index = 0; Index < UniformBuffers.Size(); ++Index)
		{
			if (UniformBuffers[Index].Type == EUniformBaseType::UB_INCLUDED_STRUCT || UniformBuffers[Index].Type == EUniformBaseType::UB_REFERENCED_STRUCT)
			{
				continue;
			}
			
		}

		const WEngine::WArray<ShaderParametersLayout::ResourceInfo>& Textures = Layout.GetTextures();

	}
}