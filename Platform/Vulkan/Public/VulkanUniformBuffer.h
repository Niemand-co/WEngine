#pragma once
#include "RHI/Public/RHIBuffer.h"

namespace Vulkan
{

	class VulkanUniformBuffer : public RHIUniformBuffer
	{
	public:

		VulkanUniformBuffer(class VulkanDevice* pInDevice, uint8* InContents, const ShaderParametersLayout* Layout, EUniformBufferUsage InUsage);

		virtual ~VulkanUniformBuffer();

		virtual void* GetBufferRHIBase() { return this; }

		VkBuffer GetHandle() const { return Buffer; }

	private:

		VulkanDevice* pDevice;

		VkBuffer Buffer;

		EUniformBufferUsage Usage;

		WEngine::WArray<RHIResource*> ResourceTable;

		VulkanAllocation Allocation;

	};

}