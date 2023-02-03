#pragma once

namespace Vulkan
{
	class VulkanDevice;
}

class RHIBufferDescriptor
{

	friend class RHIContext;
	friend class Vulkan::VulkanDevice;

public:

	RHIBufferDescriptor();

	~RHIBufferDescriptor() = default;

public:

	size_t stride;

	size_t count;

};