#pragma once
#include "RHI/Public/RHIBuffer.h"

namespace Vulkan
{

	class VulkanBufferBase
	{
	public:

		VulkanBufferBase(class VulkanDevice* pInDevice, RHIBufferDescriptor* pDescriptor);

		virtual ~VulkanBufferBase();

		void* Lock(uint32 Size, uint32 Offset, bool bRead = false);

		void Unlock(bool bRead = false);

		VkBuffer GetHandle() const { return Buffer; }

		static VulkanBufferBase* Cast(RHIBuffer* Buffer)
		{
			return static_cast<VulkanBufferBase*>(Buffer->GetBufferRHIBase());
		}

	protected:

		VkBuffer Buffer;

		VulkanDevice *pDevice;

		class VulkanAllocation *Allocation;

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

	class VulkanUniformBuffer : public RHIUniformBuffer, public VulkanBufferBase
	{
	public:

		VulkanUniformBuffer(VulkanDevice* pInDevice, RHIBufferDescriptor* pDescriptor);

		virtual ~VulkanUniformBuffer();

		virtual void* GetBufferRHIBase() override
		{
			VulkanBufferBase* Base = static_cast<VulkanBufferBase*>(this);
			return  Base;
		}

	};

	class VulkanDynamicUniformBuffer : public RHIDynamicUniformBuffer, public VulkanBufferBase
	{
	public:

		VulkanDynamicUniformBuffer(VulkanDevice* pInDevice, RHIBufferDescriptor* pDescriptor);

		virtual ~VulkanDynamicUniformBuffer();

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

	class VulkanStagingBuffer : public VulkanBufferBase
	{
	public:

		VulkanStagingBuffer(VulkanDevice* pInDevice, RHIBufferDescriptor* pDescriptor);

		virtual ~VulkanStagingBuffer();

		void* Map();

		void UnMap();

	private:

		void *MappedPointer;

	}

}