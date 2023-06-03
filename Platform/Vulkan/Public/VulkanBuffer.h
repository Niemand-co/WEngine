#pragma once
#include "RHI/Public/RHIBuffer.h"
#include "Platform/Vulkan/Allocator/Public/VulkanMemoryManager.h"

class RHIBufferDescriptor;

namespace Vulkan
{

	class VulkanBufferBase
	{
	public:

		VulkanBufferBase(class VulkanDevice* pInDevice, RHIBufferDescriptor* pDescriptor);

		virtual ~VulkanBufferBase();

		void* Lock(uint32 Size, uint32 Offset, bool bRead = false);

		void Unlock();

		VkBuffer GetHandle() const { return Allocation.GetBufferHandle(); }

		static VulkanBufferBase* Cast(RHIBuffer* Buffer)
		{
			return static_cast<VulkanBufferBase*>(Buffer->GetBufferRHIBase());
		}

	protected:

		struct PendingEntry
		{
			uint32 Size = 0;
			uint32 Offset = 0;
			uint8 bRead : 1;
			class VulkanStagingBuffer* PendingBuffer = 0;
		};

		VkBuffer Buffer;

		VulkanDevice *pDevice;

		VulkanAllocation Allocation;

		PendingEntry PendingLock;

	};

	class VulkanVertexBuffer : public RHIVertexBuffer, public VulkanBufferBase
	{
	public:

		VulkanVertexBuffer(VulkanDevice *pInDevice, RHIBufferDescriptor *pDescriptor);

		virtual ~VulkanVertexBuffer();

		virtual void* GetBufferRHIBase() override
		{ 
			VulkanBufferBase *Base = static_cast<VulkanBufferBase*>(this);
			return  Base;
		}

	};

	class VulkanIndexBuffer : public RHIIndexBuffer, public VulkanBufferBase
	{
	public:

		VulkanIndexBuffer(VulkanDevice* pInDevice, RHIBufferDescriptor* pDescriptor);

		virtual ~VulkanIndexBuffer();

		virtual void* GetBufferRHIBase() override
		{
			VulkanBufferBase* Base = static_cast<VulkanBufferBase*>(this);
			return  Base;
		}

	};

	class VulkanTextureBuffer : public RHITextureBuffer, public VulkanBufferBase
	{
	public:

		virtual void* GetBufferRHIBase() override
		{
			VulkanBufferBase* Base = static_cast<VulkanBufferBase*>(this);
			return  Base;
		}

	};

	class VulkanStagingBuffer : public RHIBuffer
	{
	public:

		VulkanStagingBuffer(class VulkanDevice* pInDevice);

		virtual ~VulkanStagingBuffer();

		void* GetMappedPointer() { return Allocation.GetMappedPointer(pDevice); }

		void FlushMappedMemory() { Allocation.FlushMappedMemory(pDevice); }

		void InvalidateMappedMemory() { Allocation.InvalidateMappedMemory(pDevice); }

		uint32 GetSize() const { return Size; }

		VkBuffer GetHandle() const { return Buffer; }

		virtual void* GetBufferRHIBase() override
		{
			VulkanStagingBuffer* Base = static_cast<VulkanStagingBuffer*>(this);
			return  Base;
		}

	private:

		VulkanDevice *pDevice;

		VkBuffer Buffer;

		VulkanAllocation Allocation;

		uint32 Size;

		VkMemoryPropertyFlags MemoryPropertyFlags;

		friend class VulkanStagingBufferManager;

	};

}