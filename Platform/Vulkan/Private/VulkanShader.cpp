#include "pch.h"
#include "Platform/Vulkan/Public/VulkanShader.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

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