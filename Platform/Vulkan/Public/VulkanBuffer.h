#pragma once
#include "RHI/Public/RHIBuffer.h"

namespace Vulkan
{

	class VulkanBuffer : public RHIBuffer
	{
	public:

		VulkanBuffer(VkBuffer *buffer, VkDevice *device);

		virtual ~VulkanBuffer();

		VkBuffer* GetHandle();

	private:

		VkBuffer *m_pBuffer;

		VkMemoryRequirements *m_pMemoryRequiremtns;

		VkDeviceMemory *m_pDeviceMemory;

		VkDevice *m_pDevice;

	};

}