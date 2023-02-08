#include "pch.h"
#include "Platform/Vulkan/Public/VulkanShader.h"

namespace Vulkan
{

	VulkanShaderBase::VulkanShaderBase(VkShaderModule* shaderModule)
		: m_shaderModule(shaderModule)
	{
		
	}

	VulkanShaderBase::~VulkanShaderBase()
	{
	}

	VulkanVertexShader::VulkanVertexShader(VkShaderModule* shaderModule)
		: VulkanShaderBase(shaderModule)
	{
	}

	VulkanVertexShader::~VulkanVertexShader()
	{
	}

	VulkanGeometryShader::VulkanGeometryShader(VkShaderModule* shaderModule)
		: VulkanShaderBase(shaderModule)
	{
	}

	VulkanGeometryShader::~VulkanGeometryShader()
	{
	}

	VulkanPixelShader::VulkanPixelShader(VkShaderModule* shaderModule)
		: VulkanShaderBase(shaderModule)
	{
	}

	VulkanPixelShader::~VulkanPixelShader()
	{
	}

	VulkanComputeShader::VulkanComputeShader(VkShaderModule* shaderModule)
		: VulkanShaderBase(shaderModule)
	{
	}

	VulkanComputeShader::~VulkanComputeShader()
	{
	}

}