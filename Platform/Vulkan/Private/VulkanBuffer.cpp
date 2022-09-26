#include "pch.h"
#include "Platform/Vulkan/Public/VulkanBuffer.h"

VulkanBuffer::VulkanBuffer(VkBuffer* buffer)
	: m_pBuffer(buffer)
{
}

VulkanBuffer::~VulkanBuffer()
{
}

VkBuffer* VulkanBuffer::GetHandle()
{
	return m_pBuffer;
}
