#include "pch.h"
#include "RHI/Public/RHIHeads.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"

#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Utils/Public/Window.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanContext.h"

RHIContext* RHIContext::g_pContext = nullptr;

RHIContext::RHIContext()
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

	RHISwapchainDescriptor swapchainDescriptor = {};
	{
		swapchainDescriptor.count = 3;
		swapchainDescriptor.format = Format::B8G8R8A8_UNorm;
		swapchainDescriptor.colorSpace = ColorSpace::SRGB_Linear;
		swapchainDescriptor.presenMode = PresentMode::Immediate;
		swapchainDescriptor.instance = pInstance;
		swapchainDescriptor.presentFamilyIndex = pQueue->GetIndex();
		swapchainDescriptor.extent = { Window::cur_window->GetWidth(), Window::cur_window->GetHeight() };
	}

	m_isDisplayChagned = false;
}

void RHIContext::Init(RHIBackend backend)
{
	switch (backend)
	{
	case RHIBackend::None:
		RE_ASSERT(false, "No RHI Support.");
	case RHIBackend::Vulkan:
		g_pContext = new VulkanContext();
	default:
		break;
	}


}

WVertexBufferRHIRef RHIContext::CreateVertexBuffer(size_t stride, size_t count)
{
	RHIBufferDescriptor descriptor = {};
	{
		descriptor.count = count;
		descriptor.stride = stride;
	}
	return pDevice->CreateVertexBuffer(&descriptor);
}

WIndexBufferRHIRef RHIContext::CreateIndexBuffer(size_t count)
{
	RHIBufferDescriptor descriptor = {};
	{
		descriptor.count = count;
		descriptor.stride = sizeof(uint32);
	}
	return pDevice->CreateIndexBuffer(&descriptor);
}

WUniformBufferRHIRef RHIContext::CreateUniformBuffer(size_t stride, size_t count)
{
	RHIBufferDescriptor descriptor = {};
	{
		descriptor.count = count;
		descriptor.stride = stride;
	}
	return pDevice->CreateUniformBuffer(&descriptor);
}

WDynamicUniformBufferRHIRef* RHIContext::CreateDynamicUniformBuffer(size_t stride, size_t count)
{
	return nullptr;
}

RHIBuffer* RHIContext::CreateTextureBuffer(RHIBufferDescriptor* descriptor)
{
	//descriptor->bufferType = BUFFER_USAGE_TRANSFER_SRC;
	//return pDevice->CreateBuffer(descriptor);
	return nullptr;
}

WVertexShaderRHIRef RHIContext::CreateVertexShader(ShaderCodeBlob& blob)
{
	RHIShaderDescriptor descriptor = {};
	{
		descriptor.codeSize = blob.GetSize();
		descriptor.pCode = blob.GetCode();
		descriptor.entryName = "VSMain";
	}
	return pDevice->CreateVertexShader(&descriptor);
}

WPixelShaderRHIRef RHIContext::CreatePixelShader(ShaderCodeBlob& blob)
{
	RHIShaderDescriptor descriptor = {};
	{
		descriptor.codeSize = blob.GetSize();
		descriptor.pCode = blob.GetCode();
		descriptor.entryName = "PSMain";
	}
	return pDevice->CreatePixelShader(&descriptor);
}

WGeometryShaderRHIRef RHIContext::CreateGeometryShader(ShaderCodeBlob& blob)
{
	RHIShaderDescriptor descriptor = {};
	{
		descriptor.codeSize = blob.GetSize();
		descriptor.pCode = blob.GetCode();
		descriptor.entryName = "GSMain";
	}
	return pDevice->CreateGeometryShader(&descriptor);
}

WComputeShaderRHIRef RHIContext::CreateComputeShader(ShaderCodeBlob& blob)
{
	RHIShaderDescriptor descriptor = {};
	{
		descriptor.codeSize = blob.GetSize();
		descriptor.pCode = blob.GetCode();
		descriptor.entryName = "CSMain";
	}
	return pDevice->CreateComputeShader(&descriptor);
}

WTexture2DRHIRef RHIContext::CreateTexture2D(uint32 InWidth, uint32 InHeight, Format InFormat, uint32 InMipCount, ETextureCreateFlags InFlag)
{
	RHITextureDescriptor descriptor = {};
	{
		descriptor.width = InWidth;
		descriptor.height = InHeight;
		descriptor.format = InFormat;
		descriptor.layerCount = 1;
		descriptor.mipCount = InMipCount;
		descriptor.Flag = InFlag;
	}
	return pDevice->CreateTexture2D(&descriptor);
}

WTexture2DArrayRHIRef RHIContext::CreateTexture2DArray(uint32 InWidth, uint32 InHeight, Format InFormat, uint32 InMipCount, uint32 InLayerCount, ETextureCreateFlags InFlag)
{
	RHITextureDescriptor descriptor = {};
	{
		descriptor.width = InWidth;
		descriptor.height = InHeight;
		descriptor.format = InFormat;
		descriptor.layerCount = InLayerCount;
		descriptor.mipCount = InMipCount;
		descriptor.Flag = InFlag;
	}
	return pDevice->CreateTexture2DArray(&descriptor);
}

WTexture3DRHIRef RHIContext::CreateTexture3D(uint32 InWidth, uint32 InHeight, uint32 InDepth, Format InFormat, uint32 InMipCount, ETextureCreateFlags InFlag)
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
	}
	return pDevice->CreateTexture3D(&descriptor);
}

WTextureViewRHIRef RHIContext::CreateTextureView(uint32 InMipIndex, uint32 InMipCount, uint32 InLayerIndex, uint32 InLayerCount, uint32 InPlaneIndex, uint32 InPlaneCount, Dimension InDimension, Format InFormat, RHITexture* InTexture)
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

WRenderPassRHIRef RHIContext::CreateRenderPass(RHIRenderPassDescriptor* descriptor)
{
	return pDevice->CreateRenderPass(descriptor);
}

RHIScissor* RHIContext::CreateScissor(RHIScissorDescriptor* descriptor)
{
	return pDevice->CreateScissor(descriptor);
}

RHIViewport* RHIContext::CreateViewport(RHIViewportDescriptor* descriptor)
{
	return pDevice->CreateViewport(descriptor);
}

void RHIContext::CopyBufferToImage(RHITexture* pTexture, RHIBuffer* pBuffer, unsigned int width, unsigned int height)
{

}

RHIGroupLayout* RHIContext::CreateGroupLayout(RHIGroupLayoutDescriptor* descriptor)
{
	return pDevice->CreateGroupLayout(descriptor);
}

WEngine::WArray<RHIGroup*> RHIContext::CreateResourceGroup(RHIGroupDescriptor* descriptor)
{
	RHIGroupPoolDescriptor poolDescriptor = {};
	{
		poolDescriptor.pGroupLayout = descriptor->pGroupLayout;
		poolDescriptor.maxSetCount = descriptor->count;
	}
	RHIGroupPool *pool = pDevice->CreateGroupPool(&poolDescriptor);

	WEngine::WArray<RHIGroup*> group = pool->GetGroup(descriptor->count);

	return group;
}

void RHIContext::UpdateUniformResourceToGroup(RHIUpdateResourceDescriptor* descriptor)
{
	pDevice->UpdateUniformResourceToGroup(descriptor);
}

void RHIContext::UpdateTextureResourceToGroup(RHIUpdateResourceDescriptor* descriptor)
{
	pDevice->UpdateTextureResourceToGroup(descriptor);
}

RHIPipelineResourceLayout* RHIContext::CreatePipelineResourceLayout(RHIPipelineResourceLayoutDescriptor* descriptor)
{
	return pDevice->CreatePipelineResourceLayout(descriptor);
}

RHIPipelineStateObject* RHIContext::CreatePSO(RHIPipelineStateObjectDescriptor* descriptor)
{
	return pDevice->CreatePipelineStateObject(descriptor);
}