#include "pch.h"
#include "Platform/Vulkan/Public/VulkanUniformBuffer.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Allocator/Public/VulkanMemoryManager.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"
#include "Render/Public/RenderDependencyGraphParameter.h"

namespace Vulkan
{

	VulkanUniformBuffer::VulkanUniformBuffer(VulkanDevice* pInDevice, uint8* InContents, const ShaderParametersLayout* InLayout, EUniformBufferUsage InUsage)
		: RHIUniformBuffer(InLayout),
		  pDevice(pInDevice)
	{
		const WEngine::WArray<ShaderParametersLayout::ResourceInfo>& Resources = InLayout->GetResources();
		const uint32 NumResources = Resources.Size();
		if (NumResources > 0)
		{
			ResourceTable.Clear();
			ResourceTable.AddZerod(NumResources);

			if (InContents)
			{
				for (uint32 Index = 0; Index < NumResources; ++Index)
				{
					ResourceTable[Index] = GetShaderParameterRHI(InContents, Resources[Index].Offset, Resources[Index].Type);
				}
			}
		}
		
		uint32 BufferSize = InLayout->GetSize();
		if (BufferSize > 0)
		{
			const bool bInRenderThread = IsRenderThread();
			const bool bInRHIThread = IsRHIThread();

			pDevice->GetMemoryManager()->AllocateUniformBuffer(Allocation, BufferSize, InContents);
		}
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		pDevice->GetMemoryManager()->FreeUniformBuffer(Allocation);
	}

}