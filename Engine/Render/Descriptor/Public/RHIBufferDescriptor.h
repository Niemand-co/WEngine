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

	size_t dataSize;

	size_t count;

	unsigned short memoryType;

	bool isDynamic;

private:

	unsigned int bufferType;

};