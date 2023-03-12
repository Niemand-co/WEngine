#include "pch.h"
#include "Platform/Vulkan/Public/VulkanBuffer.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Allocator/Public/VulkanMemoryManager.h"
#include "Platform/Vulkan/Public/VulkanGPU.h"

namespace Vulkan
{

	VulkanBufferBase::VulkanBufferBase(VulkanDevice* pInDevice, RHIBufferDescriptor* pDescriptor)
		: pDevice(pInDevice)
	{
		const bool bDynamic = WEngine::EnumHasFlags(pDescriptor->Usage, EBufferUsageFlags::BF_IndexBuffer);
		const bool bUniform = WEngine::EnumHasFlags(pDescriptor->Usage, EBufferUsageFlags::BF_UniformBuffer);
		const bool bRenderResource = WEngine::EnumHasFlags(pDescriptor->Usage, EBufferUsageFlags::BF_SRV);
		const bool bCopySrc = WEngine::EnumHasFlags(pDescriptor->Usage, EBufferUsageFlags::BF_TransferSrc);
		const bool bCopyDst = WEngine::EnumHasFlags(pDescriptor->Usage, EBufferUsageFlags::BF_TransferDst);
		const bool bIndirect = WEngine::EnumHasFlags(pDescriptor->Usage, EBufferUsageFlags::BF_IndirectBuffer);
		const bool bCPUAccess = WEngine::EnumHasFlags(pDescriptor->Usage, EBufferUsageFlags::BF_CPUAccessable);

		VkBufferUsageFlags BufferUsageFlags = bUniform ? VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT : (WEngine::EnumHasFlags(pDescriptor->Usage, EBufferUsageFlags::BF_VertexBuffer) ? VK_BUFFER_USAGE_VERTEX_BUFFER_BIT : VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

		BufferUsageFlags |= bCopySrc ? VK_BUFFER_USAGE_TRANSFER_SRC_BIT : 0;
		BufferUsageFlags |= bCopyDst ? VK_BUFFER_USAGE_TRANSFER_DST_BIT : 0;
		BufferUsageFlags |= (bRenderResource && !bUniform) ? VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT : 0;
		BufferUsageFlags |= bIndirect ? VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT : 0;
		BufferUsageFlags |= bCPUAccess ? VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT : 0;

		VkMemoryPropertyFlags MemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		MemoryPropertyFlags |= bCPUAccess ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : 0;

		pDevice->GetMemoryManager()->AllocateBuffer(Buffer, Allocation, BufferUsageFlags, MemoryPropertyFlags);


	}

	VulkanBufferBase::~VulkanBufferBase()
	{
		pDevice->GetMemoryManager()->DeallocateBuffer(Buffer, Allocation);
	}

	void* VulkanBufferBase::Lock(uint32 Size, uint32 Offset, bool bRead)
	{
		if (bRead)
		{

		}
		else
		{
			
		}
	}

	void VulkanBufferBase::Unlock(bool bRead)
	{
	}

	VulkanVertexBuffer::VulkanVertexBuffer(VulkanDevice* pInDevice, RHIBufferDescriptor* pDescriptor)
		: RHIVertexBuffer(pDescriptor->Count, pDescriptor->Stride, pDescriptor->Usage), VulkanBufferBase(pInDevice, pDescriptor)
	{
		
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
	}

	VulkanDynamicVertexBuffer::VulkanDynamicVertexBuffer(VulkanDevice* pInDevice, RHIBufferDescriptor* pDescriptor)
		: RHIDynamicVertexBuffer(pDescriptor->Count, pDescriptor->Stride, pDescriptor->Usage), VulkanBufferBase(pInDevice, pDescriptor)
	{
	}

	VulkanDynamicVertexBuffer::~VulkanDynamicVertexBuffer()
	{
	}

	VulkanIndexBuffer::VulkanIndexBuffer(VulkanDevice* pInDevice, RHIBufferDescriptor* pDescriptor)
		: RHIIndexBuffer(pDescriptor->Count, pDescriptor->Stride, pDescriptor->Usage), VulkanBufferBase(pInDevice, pDescriptor)
	{
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
	}

	VulkanUniformBuffer::VulkanUniformBuffer(VulkanDevice* pInDevice, RHIBufferDescriptor* pDescriptor)
		: RHIUniformBuffer(pDescriptor->Count, pDescriptor->Stride, pDescriptor->Usage), VulkanBufferBase(pInDevice, pDescriptor)
	{
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
	}

	VulkanDynamicUniformBuffer::VulkanDynamicUniformBuffer(VulkanDevice* pInDevice, RHIBufferDescriptor* pDescriptor)
		: RHIDynamicUniformBuffer(pDescriptor->Count, pDescriptor->Stride, pDescriptor->Usage), VulkanBufferBase(pInDevice, pDescriptor)
	{
	}

	VulkanDynamicUniformBuffer::~VulkanDynamicUniformBuffer()
	{
	}

	VulkanStagingBuffer::VulkanStagingBuffer(VulkanDevice* pInDevice, RHIBufferDescriptor* pDescriptor)
		: VulkanBufferBase(pInDevice, pDescriptor)
	{
	}

	VulkanStagingBuffer::~VulkanStagingBuffer()
	{
	}

	void* VulkanStagingBuffer::Map()
	{
		return nullptr;
	}

	void VulkanStagingBuffer::UnMap()
	{
	}

}
