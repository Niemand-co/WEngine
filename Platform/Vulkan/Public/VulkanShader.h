#pragma once
#include "RHI/Public/RHIShader.h"

namespace Vulkan
{

	class VulkanShaderCodeHeader
	{
	public:

		struct FResourceInfo
		{
			uint16 ResourceIndex;
			uint16 UBBaseType;
		};

		struct FUniformBufferInfo
		{
			uint32 LayoutHash;
			WEngine::WArray<FResourceInfo> ResourceEntries;
		};
		WEngine::WArray<FUniformBufferInfo> UniformBuffers;

	};

	class VulkanShaderBase : public VulkanResource
	{
	public:
	
		VulkanShaderBase(class VulkanDevice *pInDevice, VkShaderModuleCreateInfo* pInfo);

		virtual ~VulkanShaderBase();

		VkShaderModule GetShaderModule() const { return ShaderModule; }

		const VulkanShaderCodeHeader& GetCodeHeader() const { return CodeHeader; }

	protected:

		VulkanDevice *pDevice;

		VkShaderModule ShaderModule;

		VulkanShaderCodeHeader CodeHeader;

	};

	class VulkanVertexShader : public RHIVertexShader, public VulkanShaderBase
	{
	public:

		VulkanVertexShader(VulkanDevice* pInDevice, VkShaderModuleCreateInfo* pInfo);

		virtual ~VulkanVertexShader();

		virtual void SetupParametersInternal(const ShaderParametersLayout& Layout) override;

	};

	class VulkanGeometryShader : public RHIGeometryShader, public VulkanShaderBase
	{
	public:

		VulkanGeometryShader(VulkanDevice* pInDevice, VkShaderModuleCreateInfo* pInfo);

		virtual ~VulkanGeometryShader();

		virtual void SetupParametersInternal(const ShaderParametersLayout& Layout) override;

	};

	class VulkanPixelShader : public RHIPixelShader, public VulkanShaderBase
	{
	public:

		VulkanPixelShader(VulkanDevice* pInDevice, VkShaderModuleCreateInfo* pInfo);

		virtual ~VulkanPixelShader();

		virtual void SetupParametersInternal(const ShaderParametersLayout& Layout) override;

	};

	class VulkanComputeShader : public RHIComputeShader, public VulkanShaderBase
	{
	public:

		VulkanComputeShader(VulkanDevice* pInDevice, VkShaderModuleCreateInfo* pInfo);

		virtual ~VulkanComputeShader();

		virtual void SetupParametersInternal(const ShaderParametersLayout& Layout) override;

	};

}