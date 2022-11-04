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

	uint64_t size;

	void *pData;

	unsigned short memoryType;

private:

	unsigned int bufferType;

};