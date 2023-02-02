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

	VulkanVertexShader::VulkanVertexShader()
	{
		VkShaderModuleCreateInfo info = {};
		{
			info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			info.pCode
		}
		vkCreateShaderModule()
	}

	VulkanVertexShader::~VulkanVertexShader()
	{
	}

}