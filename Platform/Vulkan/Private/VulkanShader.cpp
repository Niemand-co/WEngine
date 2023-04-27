#include "pch.h"
#include "Platform/Vulkan/Public/VulkanShader.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Render/Public/RenderDependencyGraphParameter.h"

namespace Vulkan
{

	VulkanShaderBase::VulkanShaderBase(VulkanDevice *pInDevice, VkShaderModuleCreateInfo* pInfo)
		: pDevice(pInDevice)
	{
		vkCreateShaderModule(pDevice->GetHandle(), pInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &ShaderModule);
	}

	VulkanShaderBase::~VulkanShaderBase()
	{
		vkDestroyShaderModule(pDevice->GetHandle(), ShaderModule, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	VulkanVertexShader::VulkanVertexShader(VulkanDevice* pInDevice, VkShaderModuleCreateInfo* pInfo)
		: VulkanShaderBase(pInDevice, pInfo)
	{
	}

	VulkanVertexShader::~VulkanVertexShader()
	{
	}

	void VulkanVertexShader::SetupParametersInternal(const ShaderParametersLayout& Layout)
	{
		const WEngine::WArray<ShaderParametersLayout::ResourceInfo>& UniformBuffers =  Layout.GetUniforms();
		VulkanShaderCodeHeader::FUniformBufferInfo& UBInfo = CodeHeader.UniformBuffers.AddInitialized();
		for (int32 Index = 0; Index < UniformBuffers.Size(); ++Index)
		{
			VulkanShaderCodeHeader::FResourceInfo Resource;
			Resource.ResourceIndex = Index;
			Resource.UBBaseType = (uint16)UniformBuffers[Index].Type;
			UBInfo.ResourceEntries.Push(Resource);
		}

		const WEngine::WArray<ShaderParametersLayout::ResourceInfo>& Textures = Layout.GetTextures();

	}

	VulkanGeometryShader::VulkanGeometryShader(VulkanDevice* pInDevice, VkShaderModuleCreateInfo* pInfo)
		: VulkanShaderBase(pInDevice, pInfo)
	{
	}

	VulkanGeometryShader::~VulkanGeometryShader()
	{
	}

	VulkanPixelShader::VulkanPixelShader(VulkanDevice* pInDevice, VkShaderModuleCreateInfo* pInfo)
		: VulkanShaderBase(pInDevice, pInfo)
	{
	}

	VulkanPixelShader::~VulkanPixelShader()
	{
	}

	VulkanComputeShader::VulkanComputeShader(VulkanDevice* pInDevice, VkShaderModuleCreateInfo *pInfo)
		: VulkanShaderBase(pInDevice, pInfo)
	{
	}

	VulkanComputeShader::~VulkanComputeShader()
	{
	}

}