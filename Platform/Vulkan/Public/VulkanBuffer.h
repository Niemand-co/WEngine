#pragma once
#include "RHI/Public/RHIBuffer.h"

namespace Vulkan
{

	class VulkanBufferBase
	{
	public:

		VulkanBufferBase(VkBuffer* buffer, VkDevice* device, unsigned int memoryHeapIndex, bool isDynamic, VkDeviceSize size);

		VulkanBufferBase(VkBuffer* buffer, VkDevice* device, unsigned int memoryHeapIndex, bool isDynamic, size_t dynamicAlignment, size_t count);

		virtual ~VulkanBufferBase();

		VkBuffer* GetHandle();

	private:

		VkBuffer *pBuffer;

		VkMemoryRequirements *pMemoryRequirements;

		VkDeviceMemory *pDeviceMemory;

		void *pData;

		class VulkanDevice *pDevice;

	};

	class VulkanVertexBuffer : public RHIVertexBuffer, public VulkanBufferBase
	{
	public:

		VulkanVertexBuffer(VulkanDevice *pInDevice);

		virtual ~VulkanVertexBuffer();

	};

	class VulkanIndexBuffer : public RHIIndexBuffer, public VulkanBufferBase
	{
	public:

		VulkanIndexBuffer();

		virtual ~VulkanIndexBuffer();

	};

	class VulkanUniformBuffer : public RHIUniformBuffer, public VulkanBufferBase
	{
	public:

		VulkanUniformBuffer();

		virtual ~VulkanUniformBuffer();

	};

}