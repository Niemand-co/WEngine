#include "pch.h"
#include "Platform/Vulkan/Public/VulkanBuffer.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanGPU.h"

namespace Vulkan
{

	VulkanBufferBase::VulkanBufferBase(VulkanDevice *pInDevice, VkBufferCreateInfo *pInfo, uint32 memoryType)
		: pDevice(pInDevice)
	{
		vkCreateBuffer(*pInDevice->GetHandle(), pInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &Buffer);

		vkGetBufferMemoryRequirements(*pInDevice->GetHandle(), Buffer, &MemoryRequirements);

		unsigned int index = 0;
		GPUFeature feature = pInDevice->GetGPU()->GetFeature();
		for (; index < feature.memorySupports.Size(); ++index)
		{
			if (feature.memorySupports[index]->type == MemoryType::LocalMemory && (feature.memorySupports[index]->properties & memoryType) == memoryType)
			{
				break;
			}
		}
		RE_ASSERT(feature.memorySupports.Size() > index, "No Suitable Memory Heap Exists.");

		VkMemoryAllocateInfo MemoryAllocateInfo = {};
		{
			MemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			MemoryAllocateInfo.allocationSize = MemoryRequirements.size;
			MemoryAllocateInfo.memoryTypeIndex = index;
		}

		RE_ASSERT(vkAllocateMemory(*pInDevice->GetHandle(), &MemoryAllocateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &DeviceMemory) == VK_SUCCESS, "Failed to Allocate Memory.");

		vkBindBufferMemory(*pInDevice->GetHandle(), Buffer, DeviceMemory, 0);
	}

	VulkanBufferBase::~VulkanBufferBase()
	{
		vkDestroyBuffer(*pDevice->GetHandle(), Buffer, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
		vkFreeMemory(*pDevice->GetHandle(), DeviceMemory, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	VulkanVertexBuffer::VulkanVertexBuffer(VulkanDevice* pInDevice, VkBufferCreateInfo *pInfo, uint32 memoryType)
		: VulkanBufferBase(pInDevice, pInfo, memoryType)
	{
		
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
	}

	VulkanDynamicVertexBuffer::VulkanDynamicVertexBuffer(VulkanDevice* pInDevice, VkBufferCreateInfo* pInfo, uint32 memoryType)
		: VulkanBufferBase(pInDevice, pInfo, memoryType)
	{
	}

	VulkanDynamicVertexBuffer::~VulkanDynamicVertexBuffer()
	{
	}

	VulkanIndexBuffer::VulkanIndexBuffer(VulkanDevice* pInDevice, VkBufferCreateInfo* pInfo, uint32 memoryType)
		: VulkanBufferBase(pInDevice, pInfo, memoryType)
	{
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
	}

	VulkanUniformBuffer::VulkanUniformBuffer(VulkanDevice* pInDevice, VkBufferCreateInfo* pInfo, uint32 memoryType)
		: VulkanBufferBase(pInDevice, pInfo, memoryType)
	{
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
	}

	VulkanDynamicUniformBuffer::VulkanDynamicUniformBuffer(VulkanDevice* pInDevice, VkBufferCreateInfo* pInfo, uint32 memoryType)
		: VulkanBufferBase(pInDevice, pInfo, memoryType)
	{
	}

	VulkanDynamicUniformBuffer::~VulkanDynamicUniformBuffer()
	{
	}

}
