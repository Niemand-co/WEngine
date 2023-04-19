#pragma once
#include "RHI/Public/RHIBuffer.h"

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

		VkBuffer GetHandle() const { return Buffer; }

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

		class VulkanAllocation *Allocation;

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

	class VulkanDynamicVertexBuffer : public RHIDynamicVertexBuffer, public VulkanBufferBase
	{
	public:

		VulkanDynamicVertexBuffer(VulkanDevice* pInDevice, RHIBufferDescriptor* pDescriptor);

		virtual ~VulkanDynamicVertexBuffer();

		virtual void* GetBufferRHIBase() override
		{
			VulkanBufferBase* Base = static_cast<VulkanBufferBase*>(this);
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

	class VulkanUniformBuffer : public RHIUniformBuffer
	{
	public:

		VulkanUniformBuffer(VulkanDevice* pInDevice, RHIBufferDescriptor* pDescriptor);

		virtual ~VulkanUniformBuffer();

		virtual void* GetBufferRHIBase() { return this; }

	private:

		VulkanDevice *pDevice;

	};

	class VulkanDynamicUniformBuffer : public RHIDynamicUniformBuffer
	{
	public:

		VulkanDynamicUniformBuffer(VulkanDevice* pInDevice, RHIBufferDescriptor* pDescriptor);

		virtual ~VulkanDynamicUniformBuffer();

		virtual void* GetBufferRHIBase() { return this; }

	private:

		VulkanDevice *pDevice;

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

	class VulkanStagingBuffer : public RHIResource
	{
	public:

		VulkanStagingBuffer(class VulkanDevice* pInDevice);

		virtual ~VulkanStagingBuffer();

		void Map(uint32 Size, uint32 Offset);

		void UnMap();

		void* GetMappedPointer() const { return MappedPointer; }

		uint32 GetSize() const { return Size; }

		VkBuffer GetHandle() const { return Buffer; }

	private:

		VulkanDevice *pDevice;

		VkBuffer Buffer;

		uint32 Size;

		VkMemoryPropertyFlags MemoryPropertyFlags;

		void *MappedPointer;

		friend class VulkanStagingBufferManager;

	};

}