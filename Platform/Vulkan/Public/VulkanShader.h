#pragma once
#include "RHI/Public/RHIShader.h"

namespace Vulkan
{

	class VulkanShader : public RHIShader
	{
	public:
	
		VulkanShader(VkShaderModule *shaderModule, unsigned int stage, const WEngine::WString& entryName);

		virtual ~VulkanShader();

		VkShaderModule* GetShaderModule();

	private:

		VkShaderModule *m_shaderModule;

	};

}