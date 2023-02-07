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

}