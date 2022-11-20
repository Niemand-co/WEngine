#pragma once
#include "RHI/Public/RHIBuffer.h"

namespace Vulkan
{

	class VulkanBuffer : public RHIBuffer
	{
	public:

		VulkanBuffer(VkBuffer* buffer, VkDevice* device, unsigned int memoryHeapIndex, bool isDynamic, VkDeviceSize size);

		VulkanBuffer(VkBuffer* buffer, VkDevice* device, unsigned int memoryHeapIndex, bool isDynamic, size_t dynamicAlignment, size_t count);

		virtual ~VulkanBuffer();

		virtual void LoadData(void *pData, size_t size, size_t offset = 0) override;

		virtual void Flush(size_t range) override;

		virtual void Resize(size_t count) override;

		VkBuffer* GetHandle();

	private:

		VkBuffer *m_pBuffer;

		VkMemoryRequirements *m_pMemoryRequirements;

		VkDeviceMemory *m_pDeviceMemory;

		VkDevice *m_pDevice;

		void *m_pData;

	};

}