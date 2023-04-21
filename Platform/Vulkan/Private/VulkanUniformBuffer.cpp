#include "pch.h"
#include "Platform/Vulkan/Public/VulkanUniformBuffer.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"

namespace Vulkan
{

	VulkanUniformBuffer::VulkanUniformBuffer(VulkanDevice* pInDevice, uint8* InContent, const ShaderParametersLayout* InLayout, EBufferUsageFlags InUsage)
		: RHIUniformBuffer(InLayout),
		  pDevice(pInDevice)
	{
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
	}

}