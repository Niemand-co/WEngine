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

	WEngine::WArray<RHIQueueDescriptor> queueDescriptors(1, RHIQueueDescriptor());
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

WVertexBufferRHIRef DynamicRHIContext::CreateVertexBuffer(uint8 *InContents, uint32 InStride, uint32 InCount, EBufferUsageFlags InUsage)
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

WIndexBufferRHIRef DynamicRHIContext::CreateIndexBuffer(uint8* InContents, uint32 InCount, EBufferUsageFlags InUsage)
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

WUniformBufferRHIRef DynamicRHIContext::CreateUniformBuffer(uint8* InContents, const ShaderParametersLayout* InLayout, EBufferUsageFlags InUsage)
{
	return pDevice->CreateUniformBuffer(InContents, InLayout, InUsage);
}

WVertexShaderRHIRef DynamicRHIContext::CreateVertexShader(ShaderCodeBlob& blob)
{
	RHIShaderDescriptor descriptor = {};
	{
		descriptor.codeSize = blob.GetSize();
		descriptor.pCode = blob.GetCode();
		descriptor.entryName = "VSMain";
	}
	return pDevice->CreateVertexShader(&descriptor);
}

WPixelShaderRHIRef DynamicRHIContext::CreatePixelShader(ShaderCodeBlob& blob)
{
	RHIShaderDescriptor descriptor = {};
	{
		descriptor.codeSize = blob.GetSize();
		descriptor.pCode = blob.GetCode();
		descriptor.entryName = "PSMain";
	}
	return pDevice->CreatePixelShader(&descriptor);
}

WGeometryShaderRHIRef DynamicRHIContext::CreateGeometryShader(ShaderCodeBlob& blob)
{
	RHIShaderDescriptor descriptor = {};
	{
		descriptor.codeSize = blob.GetSize();
		descriptor.pCode = blob.GetCode();
		descriptor.entryName = "GSMain";
	}
	return pDevice->CreateGeometryShader(&descriptor);
}

WComputeShaderRHIRef DynamicRHIContext::CreateComputeShader(ShaderCodeBlob& blob)
{
	RHIShaderDescriptor descriptor = {};
	{
		descriptor.codeSize = blob.GetSize();
		descriptor.pCode = blob.GetCode();
		descriptor.entryName = "CSMain";
	}
	return pDevice->CreateComputeShader(&descriptor);
}

WTexture2DRHIRef DynamicRHIContext::CreateTexture2D(uint32 InWidth, uint32 InHeight, Format InFormat, uint32 InMipCount, ClearValue InClearValue, ETextureCreateFlags InFlag, EAccess InitState)
{
	RHITextureDescriptor descriptor = {};
	{
		descriptor.width = InWidth;
		descriptor.height = InHeight;
		descriptor.format = InFormat;
		descriptor.layerCount = 1;
		descriptor.mipCount = InMipCount;
		descriptor.Flag = InFlag;
		descriptor.initialState = InitState;
		descriptor.clearValue = InClearValue;
	}
	return pDevice->CreateTexture2D(&descriptor);
}

WTexture2DArrayRHIRef DynamicRHIContext::CreateTexture2DArray(uint32 InWidth, uint32 InHeight, Format InFormat, uint32 InMipCount, uint32 InLayerCount, ClearValue InClearValue, ETextureCreateFlags InFlag, EAccess InitState)
{
	RHITextureDescriptor descriptor = {};
	{
		descriptor.width = InWidth;
		descriptor.height = InHeight;
		descriptor.format = InFormat;
		descriptor.layerCount = InLayerCount;
		descriptor.mipCount = InMipCount;
		descriptor.Flag = InFlag;
		descriptor.initialState = InitState;
		descriptor.clearValue = InClearValue;
	}
	return pDevice->CreateTexture2DArray(&descriptor);
}

WTexture3DRHIRef DynamicRHIContext::CreateTexture3D(uint32 InWidth, uint32 InHeight, uint32 InDepth, Format InFormat, uint32 InMipCount, ClearValue InClearValue, ETextureCreateFlags InFlag, EAccess InitState)
{
	RHITextureDescriptor descriptor = {};
	{
		descriptor.width = InWidth;
		descriptor.height = InHeight;
		descriptor.depth = InDepth;
		descriptor.format = InFormat;
		descriptor.layerCount = 1;
		descriptor.mipCount = InMipCount;
		descriptor.Flag = InFlag;
		descriptor.initialState = InitState;
		descriptor.clearValue = InClearValue;
	}
	return pDevice->CreateTexture3D(&descriptor);
}

WTextureViewRHIRef DynamicRHIContext::CreateTextureView(uint32 InMipIndex, uint32 InMipCount, uint32 InLayerIndex, uint32 InLayerCount, uint32 InPlaneIndex, uint32 InPlaneCount, Dimension InDimension, Format InFormat, RHITexture* InTexture)
{
	RHITextureViewDescriptor descriptor = {};
	{
		descriptor.format = InFormat;
		descriptor.baseMipLevel = InMipIndex;
		descriptor.mipCount = InMipCount;
		descriptor.baseArrayLayer = InLayerIndex;
		descriptor.arrayLayerCount = InLayerCount;
		descriptor.dimension = InDimension;
	}
	return pDevice->CreateTextureView(&descriptor, InTexture);
}

void DynamicRHIContext::CopyBufferToImage(RHITexture* pTexture, RHIBuffer* pBuffer, unsigned int width, unsigned int height)
{

}

void DynamicRHIContext::UpdateUniformResourceToGroup(RHIUpdateResourceDescriptor* descriptor)
{
	pDevice->UpdateUniformResourceToGroup(descriptor);
}

void DynamicRHIContext::UpdateTextureResourceToGroup(RHIUpdateResourceDescriptor* descriptor)
{
	pDevice->UpdateTextureResourceToGroup(descriptor);
}

WViewportRHIRef DynamicRHIContext::CreateViewport(uint32 InWidth, uint32 InHeight, bool bInFullScreen, Format InFormat)
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