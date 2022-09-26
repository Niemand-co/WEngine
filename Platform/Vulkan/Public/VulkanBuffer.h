#pragma once
#include "RHI/Public/RHIBuffer.h"

class VulkanBuffer : public RHIBuffer
{
public:

	VulkanBuffer(VkBuffer *buffer);

	virtual ~VulkanBuffer();

	VkBuffer* GetHandle();

private:

	VkBuffer *m_pBuffer;

};