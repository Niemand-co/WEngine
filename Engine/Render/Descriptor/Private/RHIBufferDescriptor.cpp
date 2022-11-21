#include "pch.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

RHIBufferDescriptor::RHIBufferDescriptor()
{
	count = 1;
	isDynamic = false;
}