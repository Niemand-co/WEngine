#include "pch.h"
#include "Platform/Vulkan/Public/VulkanBuffer.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Allocator/Public/VulkanMemoryManager.h"
#include "Platform/Vulkan/Public/VulkanGPU.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Platform/Vulkan/Public/VulkanContext.h"

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

		void *Data = Lock(pDescriptor->Count * pDescriptor->Stride, 0);
		memcpy(Data, pDescriptor->Data, pDescriptor->Count * pDescriptor->Stride);
		Unlock();
	}

	VulkanBufferBase::~VulkanBufferBase()
	{
		pDevice->GetMemoryManager()->DeallocateBuffer(Buffer, Allocation);
	}

	void* VulkanBufferBase::Lock(uint32 Size, uint32 Offset, bool bRead)
	{
		while(PendingLock.PendingBuffer != nullptr);

		PendingLock.Size = Size;
		PendingLock.Offset = Offset;
		PendingLock.bRead = bRead;

		if (bRead)
		{
			pDevice->PrepareForCPURead();
			VulkanCommandBuffer *CmdBuffer = static_cast<VulkanContext*>(RHIContext::GetContext())->GetCmdBufferManager()->GetImmediateCommandBuffer();
			
			PendingLock.PendingBuffer = pDevice->GetStagingBufferManager()->AcquireBuffer(Size, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_CACHED_BIT);

			VkMemoryBarrier BarrierBefore = {};
			{
				BarrierBefore.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
				BarrierBefore.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
				BarrierBefore.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			}
			vkCmdPipelineBarrier(CmdBuffer->GetHandle(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 1, &BarrierBefore, 0, nullptr, 0, nullptr);

			VkBufferCopy Region = {};
			{
				Region.size = Size;
				Region.srcOffset = Offset;
				Region.dstOffset = 0;
			}
			vkCmdCopyBuffer(CmdBuffer->GetHandle(), Buffer, PendingLock.PendingBuffer->GetHandle(), 1, &Region);

			VkMemoryBarrier BarrierAfter = {};
			{
				BarrierAfter.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
				BarrierAfter.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
				BarrierAfter.dstAccessMask = VK_ACCESS_HOST_READ_BIT;
			}
			vkCmdPipelineBarrier(CmdBuffer->GetHandle(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 1, &BarrierAfter, 0, nullptr, 0, nullptr);

			static_cast<VulkanContext*>(RHIContext::GetContext())->GetCmdBufferManager()->SubmitImmediateCommandBuffer();
			pDevice->Wait();

			PendingLock.PendingBuffer->Map(Size, Offset);
			void *Data = PendingLock.PendingBuffer->GetMappedPointer();

			static_cast<VulkanContext*>(RHIContext::GetContext())->GetCmdBufferManager()->PrepareForNewActiveCmdBuffer();

			return Data;
		}
		else
		{
			PendingLock.PendingBuffer = pDevice->GetStagingBufferManager()->AcquireBuffer(Size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
			PendingLock.PendingBuffer->Map(Size, Offset);
			void *Data = PendingLock.PendingBuffer->GetMappedPointer();
			return Data;
		}
	}

	void VulkanBufferBase::Unlock()
	{
		RE_ASSERT(PendingLock.PendingBuffer != nullptr, "No pending buffer exist.");

		bool bRead = PendingLock.bRead;
		uint32 Size = PendingLock.Size;
		uint32 Offset = PendingLock.Offset;
		VulkanStagingBuffer *StagingBuffer = PendingLock.PendingBuffer;

		PendingLock.PendingBuffer = nullptr;

		if (bRead)
		{
			StagingBuffer->UnMap();
			pDevice->GetStagingBufferManager()->ReleaseBuffer(StagingBuffer);
		}
		else
		{
			StagingBuffer->UnMap();
			pDevice->PrepareForCPURead();

			VulkanCommandBufferManager *CmdBufferMgr = static_cast<VulkanContext*>(RHIContext::GetContext())->GetCmdBufferManager();
			VulkanCommandBuffer *CmdBuffer = CmdBufferMgr->GetImmediateCommandBuffer();
			RE_ASSERT(CmdBuffer->IsOutsideRenderPass(), "Cannot copy buffer inside render pass.");

			VkBufferCopy Region = {};
			{
				Region.size = Size;
				Region.srcOffset = 0;
				Region.dstOffset = Offset;
			}
			vkCmdCopyBuffer(CmdBuffer->GetHandle(), StagingBuffer->GetHandle(), Buffer, 1, &Region);

			VkMemoryBarrier BarrierAfter = {};
			{
				BarrierAfter.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
				BarrierAfter.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				BarrierAfter.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
			}
			vkCmdPipelineBarrier(CmdBuffer->GetHandle(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 1, &BarrierAfter, 0, nullptr, 0, nullptr);

			CmdBufferMgr->SubmitImmediateCommandBuffer();
			pDevice->Wait();

			pDevice->GetStagingBufferManager()->ReleaseBuffer(StagingBuffer);
		}
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

	VulkanStagingBuffer::VulkanStagingBuffer(VulkanDevice* pInDevice, VkBuffer& InBuffer, VulkanAllocation* InAllocation)
		: pDevice(pInDevice), Buffer(InBuffer), Allocation(InAllocation)
	{
	}

	VulkanStagingBuffer::~VulkanStagingBuffer()
	{
		vkDestroyBuffer(pDevice->GetHandle(), Buffer, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	void VulkanStagingBuffer::Map(uint32 Size, uint32 Offset)
	{
		vkMapMemory(pDevice->GetHandle(), Allocation->GetMemoryHandle(), Offset, Size, 0, &MappedPointer);
	}

	void VulkanStagingBuffer::UnMap()
	{
		vkUnmapMemory(pDevice->GetHandle(), Allocation->GetMemoryHandle());
	}

}
