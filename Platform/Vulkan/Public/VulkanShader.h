#pragma once
#include "RHI/Public/RHIShader.h"

namespace Vulkan
{

	class VulkanShaderBase : public VulkanResource
	{
	public:
	
		VulkanShaderBase(VkShaderModule *shaderModule);

		virtual ~VulkanShaderBase();

		VkShaderModule* GetShaderModule() const { return m_shaderModule; }

	private:

		VkShaderModule *m_shaderModule;

	};

	class VulkanVertexShader : public RHIVertexShader, public VulkanShaderBase
	{
	public:

		VulkanVertexShader();

		virtual ~VulkanVertexShader();

	};

	class VulkanGeometryShader : public RHIGeometryShader, public VulkanShaderBase
	{
	public:

		VulkanGeometryShader();

		virtual ~VulkanGeometryShader();

	};

	class VulkanPixelShader : public RHIPixelShader, public VulkanShaderBase
	{
	public:

		VulkanPixelShader();

		virtual ~VulkanPixelShader();

	};

	class VulkanComputeShader : public RHIComputeShader, public VulkanShaderBase
	{
	public:

		VulkanComputeShader();

		virtual ~VulkanComputeShader();

	};

}