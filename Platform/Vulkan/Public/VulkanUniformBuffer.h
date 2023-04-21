#pragma once
#include "RHI/Public/RHIBuffer.h"

class RHIBufferDescriptor;

namespace Vulkan
{

	class VulkanUniformBuffer : public RHIUniformBuffer
	{
	public:

		VulkanUniformBuffer(class VulkanDevice* pInDevice, uint8* InContent, const ShaderParametersLayout* Layout, EBufferUsageFlags InUsage);

		virtual ~VulkanUniformBuffer();

		virtual void* GetBufferRHIBase() { return this; }

		VkBuffer GetHandle() const { return Buffer; }

	private:

		VulkanDevice* pDevice;

		VkBuffer Buffer;

	};

}