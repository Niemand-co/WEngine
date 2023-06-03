#include "pch.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIInstance.h"
#include "RHI/Public/RHIGPU.h"
#include "RHI/Public/RHIDevice.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanContext.h"

DynamicRHIContext::DynamicRHIContext()
{
	RHIInstanceDescriptor descriptor = {};
	{
		descriptor.backend = RHIBackend::Vulkan;
		descriptor.enableDebugLayer = true;
		descriptor.enableGPUValidator = false;
	}
	pInstance = RHIInstance::CreateInstance(&descriptor);

	WEngine::WArray<RHIQueueDescriptor> queueDescriptors(1);
	{
		queueDescriptors[0].count = 1;
		queueDescriptors[0].type = RHIQueueType::Graphics;
	}
	RHIDeviceDescriptor deviceDescriptor;
	{
		deviceDescriptor.queueInfos = queueDescriptors.GetData();
		deviceDescriptor.queueInfoCount = queueDescriptors.Size();
	}

	pDevice = pInstance->GetGPU(0)->CreateDevice(&deviceDescriptor);

	pQueue = pDevice->GetQueue(RHIQueueType::Graphics, 1);
}

DynamicRHIContext::~DynamicRHIContext()
{
	delete pQueue;
	delete pDevice;
	delete pInstance;
}

WVertexBufferRHIRef DynamicRHIContext::RHICreateVertexBuffer(uint8 *InContents, uint32 InStride, uint32 InCount, EBufferUsageFlags InUsage)
{
	RHIBufferDescriptor descriptor = {};
	{
		descriptor.Count = InCount;
		descriptor.Stride = InStride;
		descriptor.Usage = InUsage;
		descriptor.Data = InContents;
	}
	return pDevice->CreateVertexBuffer(&descriptor);
}

WIndexBufferRHIRef DynamicRHIContext::RHICreateIndexBuffer(uint8* InContents, uint32 InCount, EBufferUsageFlags InUsage)
{
	RHIBufferDescriptor descriptor = {};
	{
		descriptor.Count = InCount;
		descriptor.Stride = sizeof(uint32);
		descriptor.Usage = InUsage;
		descriptor.Data = InContents;
	}
	return pDevice->CreateIndexBuffer(&descriptor);
}

WUniformBufferRHIRef DynamicRHIContext::RHICreateUniformBuffer(uint8* InContents, const ShaderParametersLayout* InLayout, EUniformBufferUsage InUsage)
{
	return pDevice->CreateUniformBuffer(InContents, InLayout, InUsage);
}

WShaderRHIRef DynamicRHIContext::RHICreateShader(EShaderFrequency InFrequency, ShaderCodeBlob& InBlob)
{
	RHIShaderDescriptor descriptor = {};
	{
		descriptor.CodeSize = InBlob.GetSize();
		descriptor.Code = InBlob.GetCode();
		descriptor.EntryName = "VSMain";
		descriptor.Frequency = InFrequency;
	}
	return pDevice->CreateShader(&descriptor);
}

WTextureRHIRef DynamicRHIContext::RHICreateTexture(const RHITextureDesc& InDesc)
{
	return pDevice->CreateTexture(InDesc);
}

WTextureViewRHIRef DynamicRHIContext::RHICreateTextureView(uint32 InMipIndex, uint32 InMipCount, uint32 InLayerIndex, uint32 InLayerCount, uint32 InPlaneIndex, uint32 InPlaneCount, EDimension InDimension, EFormat InFormat, RHITexture* InTexture)
{
	RHITextureViewDescriptor descriptor = {};
	{
		descriptor.Format = InFormat;
		descriptor.baseMipLevel = InMipIndex;
		descriptor.mipCount = InMipCount;
		descriptor.baseArrayLayer = InLayerIndex;
		descriptor.arrayLayerCount = InLayerCount;
		descriptor.Dimension = InDimension;
	}
	return nullptr;
}

void DynamicRHIContext::RHICopyBufferToImage(RHITexture* pTexture, RHIBuffer* pBuffer, unsigned int width, unsigned int height)
{

}

WViewportRHIRef DynamicRHIContext::RHICreateViewport(uint32 InWidth, uint32 InHeight, bool bInFullScreen, EFormat InFormat)
{
	RHIViewportDescriptor descriptor = {};
	{
		descriptor.format = InFormat;
		descriptor.Width = InWidth;
		descriptor.Height = InHeight;
	}
	return pDevice->CreateViewport(&descriptor);
}

RHIGPU* DynamicRHIContext::GetGPU() const
{
	return pInstance->GetGPU(0);
}

StaticRHIContext* GetStaticRHI()
{
	static StaticRHIContext *StaticRHI = new Vulkan::VulkanStaticContext;
	return StaticRHI;
}

DynamicRHIContext* GetDynamicRHI()
{
	static DynamicRHIContext *DynamicRHI = new Vulkan::VulkanDynamicContext;
	return DynamicRHI;
}