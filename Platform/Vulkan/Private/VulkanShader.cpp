#include "pch.h"
#include "Platform/Vulkan/Public/VulkanShader.h"

namespace Vulkan
{

	VulkanShader::VulkanShader(VkShaderModule* shaderModule, unsigned int stage, const WEngine::WString& entryName)
		: m_shaderModule(shaderModule)
	{
		m_stage = stage;
		m_entryName = entryName;
	}

	VulkanShader::~VulkanShader()
	{
	}

	VkShaderModule* VulkanShader::GetShaderModule()
	{
		return m_shaderModule;
	}

}