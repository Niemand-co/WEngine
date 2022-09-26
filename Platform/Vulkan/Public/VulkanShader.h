#pragma once
#include "RHI/Public/RHIShader.h"

namespace Vulkan
{

	class VulkanShader : public RHIShader
	{
	public:
	
		VulkanShader(VkShaderModule *shaderModule, ShaderStage stage, std::string entryName);

		virtual ~VulkanShader();

		VkShaderModule* GetShaderModule();

	private:

		VkShaderModule *m_shaderModule;

	};

}