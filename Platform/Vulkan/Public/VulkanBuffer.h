#pragma once
#include "RHI/Public/RHIBuffer.h"

namespace Vulkan
{

	class VulkanBufferBase
	{
	public:

		VulkanBufferBase(VulkanDevice* pInDevice, VkBufferCreateInfo *pInfo, uint32 memoryType);

		virtual ~VulkanBufferBase();

		VkBuffer* GetHandle();

	private:

		VkBuffer *pBuffer;

		VkMemoryRequirements *pMemoryRequirements;

		VkDeviceMemory *pDeviceMemory;

		void *pData;

		VkDevice *pDevice;

	};

	class VulkanVertexBuffer : public RHIVertexBuffer, public VulkanBufferBase
	{
	public:

		VulkanVertexBuffer(VulkanDevice *pInDevice, VkBufferCreateInfo * pInfo, uint32 memoryType);

		virtual ~VulkanVertexBuffer();

		virtual void LoadData(void* pData, size_t size, size_t offset = 0) override;

		virtual void Flush(size_t range) override;

		virtual void Resize(size_t count) override;

	};

	class VulkanDynamicVertexBuffer : public RHIDynamicVertexBuffer, public VulkanBufferBase
	{
	public:

		VulkanDynamicVertexBuffer(VulkanDevice* pInDevice, VkBufferCreateInfo* pInfo, uint32 memoryType);

		virtual ~VulkanDynamicVertexBuffer();

		virtual void LoadData(void* pData, size_t size, size_t offset = 0) override;

		virtual void Flush(size_t range) override;

		virtual void Resize(size_t count) override;
	};

	class VulkanIndexBuffer : public RHIIndexBuffer, public VulkanBufferBase
	{
	public:

		VulkanIndexBuffer(VulkanDevice* pInDevice, VkBufferCreateInfo* pInfo, uint32 memoryType);

		virtual ~VulkanIndexBuffer();

		virtual void LoadData(void* pData, size_t size, size_t offset = 0) override;

		virtual void Flush(size_t range) override;

		virtual void Resize(size_t count) override;

	};

	class VulkanUniformBuffer : public RHIUniformBuffer, public VulkanBufferBase
	{
	public:

		VulkanUniformBuffer(VulkanDevice* pInDevice, VkBufferCreateInfo* pInfo, uint32 memoryType);

		virtual ~VulkanUniformBuffer();

		virtual void LoadData(void* pData, size_t size, size_t offset = 0) override;

		virtual void Flush(size_t range) override;

		virtual void Resize(size_t count) override;

	};

	class VulkanDynamicUniformBuffer : public RHIDynamicUniformBuffer, public VulkanBufferBase
	{
	public:

		VulkanDynamicUniformBuffer(VulkanDevice* pInDevice, VkBufferCreateInfo* pInfo, uint32 memoryType);

		virtual ~VulkanDynamicUniformBuffer();

		virtual void LoadData(void* pData, size_t size, size_t offset = 0) override;

		virtual void Flush(size_t range) override;

		virtual void Resize(size_t count) override;
	};

}