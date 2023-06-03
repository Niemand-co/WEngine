#pragma once
#include "RHI/Public/RHIShader.h"

class RHIShaderDescriptor;

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

	class VulkanShader : public RHIShader
	{
	public:
	
		VulkanShader(class VulkanDevice *pInDevice, RHIShaderDescriptor *Descriptor);

		virtual ~VulkanShader();

		virtual void SetupParametersInternal(const ShaderParametersLayout& Layout) override;

		VkShaderModule GetShaderModule() const { return ShaderModule; }

		const VulkanShaderCodeHeader& GetCodeHeader() const { return CodeHeader; }

	protected:

		VulkanDevice *pDevice;

		VkShaderModule ShaderModule;

		VulkanShaderCodeHeader CodeHeader;

	};

}