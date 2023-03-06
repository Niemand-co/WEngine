#pragma once
#include "RHI/Public/RHIShader.h"

namespace Vulkan
{

	class VulkanShaderBase : public VulkanResource
	{
	public:
	
		VulkanShaderBase(class VulkanDevice *pInDevice, VkShaderModuleCreateInfo* pInfo);

		virtual ~VulkanShaderBase();

		VkShaderModule GetShaderModule() const { return ShaderModule; }

	private:

		VulkanDevice *pDevice;

		VkShaderModule ShaderModule;

	};

	class VulkanVertexShader : public RHIVertexShader, public VulkanShaderBase
	{
	public:

		VulkanVertexShader(VulkanDevice* pInDevice, VkShaderModuleCreateInfo* pInfo);

		virtual ~VulkanVertexShader();

	};

	class VulkanGeometryShader : public RHIGeometryShader, public VulkanShaderBase
	{
	public:

		VulkanGeometryShader(VulkanDevice* pInDevice, VkShaderModuleCreateInfo* pInfo);

		virtual ~VulkanGeometryShader();

	};

	class VulkanPixelShader : public RHIPixelShader, public VulkanShaderBase
	{
	public:

		VulkanPixelShader(VulkanDevice* pInDevice, VkShaderModuleCreateInfo* pInfo);

		virtual ~VulkanPixelShader();

	};

	class VulkanComputeShader : public RHIComputeShader, public VulkanShaderBase
	{
	public:

		VulkanComputeShader(VulkanDevice* pInDevice, VkShaderModuleCreateInfo* pInfo);

		virtual ~VulkanComputeShader();

	};

}