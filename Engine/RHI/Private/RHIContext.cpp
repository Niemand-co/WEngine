#include "pch.h"
#include "RHI/Public/RHIHeads.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Utils/Public/Window.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

RHIInstance* RHIContext::g_pInstance = nullptr;

RHIDevice* RHIContext::g_pDevice = nullptr;

RHISwapchain* RHIContext::g_pSwapchain = nullptr;

RHIContext* RHIContext::g_pContext = nullptr;

RHIQueue* RHIContext::g_pQueue = nullptr;

bool RHIContext::m_isDisplayChagned = false;

RHIContext::RHIContext()
{
}

void RHIContext::Init()
{
	g_pContext = new RHIContext();

	RHIInstanceDescriptor descriptor = {};
	{
		descriptor.backend = RHIBackend::Vulkan;
		descriptor.enableDebugLayer = true;
		descriptor.enableGPUValidator = false;
	}
	g_pInstance = RHIInstance::CreateInstance(&descriptor);

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

	g_pDevice = g_pInstance->GetGPU(0)->CreateDevice(&deviceDescriptor);

	g_pQueue = g_pDevice->GetQueue(RHIQueueType::Graphics, 1);

	RHISwapchainDescriptor swapchainDescriptor = {};
	{
		swapchainDescriptor.count = 3;
		swapchainDescriptor.format = Format::B8G8R8A8_UNorm;
		swapchainDescriptor.colorSpace = ColorSpace::SRGB_Linear;
		swapchainDescriptor.presenMode = PresentMode::Immediate;
		swapchainDescriptor.instance = g_pInstance;
		swapchainDescriptor.presentFamilyIndex = g_pQueue->GetIndex();
		swapchainDescriptor.extent = { Window::cur_window->GetWidth(), Window::cur_window->GetHeight() };
	}
	g_pSwapchain = g_pDevice->CreateSwapchain(&swapchainDescriptor);

	m_isDisplayChagned = false;
}

void RHIContext::RecreateSwapchain()
{

}

RHITexture* RHIContext::GetTexture(unsigned int index)
{
	return nullptr;
}

RHITextureView* RHIContext::GetTextureView(unsigned int index)
{
	return nullptr;
}

RHITextureView* RHIContext::GetDepthView(unsigned int index)
{
	return nullptr;
}

int RHIContext::GetNextImage(RHISemaphore *pSignalSemaphore)
{
	return g_pDevice->GetNextImage(g_pSwapchain, pSignalSemaphore);
}

void RHIContext::Submit(RHISubmitDescriptor* descriptor)
{
}

int32 RHIContext::AcquireImageIndex(RHISemaphore **OutSemaphore)
{
	return g_pSwapchain->AcquireImageIndex(OutSemaphore);
}

void RHIContext::Present(unsigned int imageIndex)
{
}

bool RHIContext::IsDisplayChanged()
{
	return m_isDisplayChagned;
}

void RHIContext::ResetDisplayState()
{
	m_isDisplayChagned = false;
}

WVertexBufferRHIRef RHIContext::CreateVertexBuffer(size_t stride, size_t count)
{
	RHIBufferDescriptor descriptor = {};
	{
		descriptor.count = count;
		descriptor.stride = stride;
	}
	return g_pDevice->CreateVertexBuffer(&descriptor);
}

WIndexBufferRHIRef RHIContext::CreateIndexBuffer(size_t count)
{
	RHIBufferDescriptor descriptor = {};
	{
		descriptor.count = count;
		descriptor.stride = sizeof(uint32);
	}
	return g_pDevice->CreateIndexBuffer(&descriptor);
}

WUniformBufferRHIRef RHIContext::CreateUniformBuffer(size_t stride, size_t count)
{
	RHIBufferDescriptor descriptor = {};
	{
		descriptor.count = count;
		descriptor.stride = stride;
	}
	return g_pDevice->CreateUniformBuffer(&descriptor);
}

WDynamicUniformBufferRHIRef* RHIContext::CreateDynamicUniformBuffer(size_t stride, size_t count)
{
	return nullptr;
}

RHIBuffer* RHIContext::CreateTextureBuffer(RHIBufferDescriptor* descriptor)
{
	//descriptor->bufferType = BUFFER_USAGE_TRANSFER_SRC;
	//return g_pDevice->CreateBuffer(descriptor);
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
	return g_pDevice->CreateVertexShader(&descriptor);
}

WPixelShaderRHIRef RHIContext::CreatePixelShader(ShaderCodeBlob& blob)
{
	RHIShaderDescriptor descriptor = {};
	{
		descriptor.codeSize = blob.GetSize();
		descriptor.pCode = blob.GetCode();
		descriptor.entryName = "PSMain";
	}
	return g_pDevice->CreatePixelShader(&descriptor);
}

WGeometryShaderRHIRef RHIContext::CreateGeometryShader(ShaderCodeBlob& blob)
{
	RHIShaderDescriptor descriptor = {};
	{
		descriptor.codeSize = blob.GetSize();
		descriptor.pCode = blob.GetCode();
		descriptor.entryName = "GSMain";
	}
	return g_pDevice->CreateGeometryShader(&descriptor);
}

WComputeShaderRHIRef RHIContext::CreateComputeShader(ShaderCodeBlob& blob)
{
	RHIShaderDescriptor descriptor = {};
	{
		descriptor.codeSize = blob.GetSize();
		descriptor.pCode = blob.GetCode();
		descriptor.entryName = "CSMain";
	}
	return g_pDevice->CreateComputeShader(&descriptor);
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
	return g_pDevice->CreateTexture2D(&descriptor);
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
	return g_pDevice->CreateTexture2DArray(&descriptor);
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
	return g_pDevice->CreateTexture3D(&descriptor);
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
	return g_pDevice->CreateTextureView(&descriptor, InTexture);
}

WRenderPassRHIRef RHIContext::CreateRenderPass(RHIRenderPassDescriptor* descriptor)
{
	return g_pDevice->CreateRenderPass(descriptor);
}

RHIScissor* RHIContext::CreateScissor(RHIScissorDescriptor* descriptor)
{
	return g_pDevice->CreateScissor(descriptor);
}

RHIViewport* RHIContext::CreateViewport(RHIViewportDescriptor* descriptor)
{
	return g_pDevice->CreateViewport(descriptor);
}

void RHIContext::CopyBufferToImage(RHITexture* pTexture, RHIBuffer* pBuffer, unsigned int width, unsigned int height)
{

}

RHIGroupLayout* RHIContext::CreateGroupLayout(RHIGroupLayoutDescriptor* descriptor)
{
	return g_pDevice->CreateGroupLayout(descriptor);
}

WEngine::WArray<RHIGroup*> RHIContext::CreateResourceGroup(RHIGroupDescriptor* descriptor)
{
	RHIGroupPoolDescriptor poolDescriptor = {};
	{
		poolDescriptor.pGroupLayout = descriptor->pGroupLayout;
		poolDescriptor.maxSetCount = descriptor->count;
	}
	RHIGroupPool *pool = g_pDevice->CreateGroupPool(&poolDescriptor);

	WEngine::WArray<RHIGroup*> group = pool->GetGroup(descriptor->count);

	return group;
}

void RHIContext::UpdateUniformResourceToGroup(RHIUpdateResourceDescriptor* descriptor)
{
	g_pDevice->UpdateUniformResourceToGroup(descriptor);
}

void RHIContext::UpdateTextureResourceToGroup(RHIUpdateResourceDescriptor* descriptor)
{
	g_pDevice->UpdateTextureResourceToGroup(descriptor);
}

RHIPipelineResourceLayout* RHIContext::CreatePipelineResourceLayout(RHIPipelineResourceLayoutDescriptor* descriptor)
{
	return g_pDevice->CreatePipelineResourceLayout(descriptor);
}

RHIPipelineStateObject* RHIContext::CreatePSO(RHIPipelineStateObjectDescriptor* descriptor)
{
	return g_pDevice->CreatePipelineStateObject(descriptor);
}