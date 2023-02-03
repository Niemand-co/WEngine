#include "pch.h"
#include "Platform/Vulkan/Public/VulkanBuffer.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanGPU.h"

namespace Vulkan
{

	VulkanBufferBase::VulkanBufferBase(VulkanDevice *pInDevice, VkBufferCreateInfo *pInfo, uint32 memoryType)
	{
		VkDevice& device = *pInDevice->GetHandle();

		pBuffer = (VkBuffer*)WEngine::Allocator::Get()->Allocate(sizeof(VkBuffer));
		vkCreateBuffer(device, pInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pBuffer);

		pMemoryRequirements = (VkMemoryRequirements*)WEngine::Allocator::Get()->Allocate(sizeof(VkMemoryRequirements));
		vkGetBufferMemoryRequirements(device, *pBuffer, pMemoryRequirements);

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

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = pMemoryRequirements->size;
		memoryAllocateInfo.memoryTypeIndex = index;

		pDeviceMemory = (VkDeviceMemory*)WEngine::Allocator::Get()->Allocate(sizeof(VkDeviceMemory));
		RE_ASSERT(vkAllocateMemory(device, &memoryAllocateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pDeviceMemory) == VK_SUCCESS, "Failed to Allocate Memory.");

		vkBindBufferMemory(*pDevice, *pBuffer, *pDeviceMemory, 0);
	}

	VulkanBufferBase::~VulkanBufferBase()
	{
		vkDestroyBuffer(*pDevice, *pBuffer, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks());
		vkFreeMemory(*pDevice, *pDeviceMemory, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks());
	}

	VkBuffer* VulkanBufferBase::GetHandle()
	{
		return pBuffer;
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
