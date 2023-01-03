#include "pch.h"
#include "RHI/Public/RHIHeads.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Utils/Public/Window.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

unsigned int RHIContext::g_maxFrames = 3;

unsigned int RHIContext::g_currentFrame = 0;

int RHIContext::g_currentImage = 0;

RHIInstance* RHIContext::g_pInstance = nullptr;

RHIDevice* RHIContext::g_pDevice = nullptr;

RHISwapchain* RHIContext::g_pSwapchain = nullptr;

RHIContext* RHIContext::g_pContext = nullptr;

RHIQueue* RHIContext::g_pQueue = nullptr;

RHISurface* RHIContext::g_pSurface = nullptr;

RHICommandPool* RHIContext::g_pPool = nullptr;

WEngine::WArray<RHISemaphore*> RHIContext::g_pImageAvailibleSemaphores = WEngine::WArray<RHISemaphore*>();

WEngine::WArray<RHISemaphore*> RHIContext::g_pPresentAVailibleSemaphores = WEngine::WArray<RHISemaphore*>();

WEngine::WArray<RHIFence*> RHIContext::g_pFences = WEngine::WArray<RHIFence*>();

WEngine::WArray<RHITextureView*> RHIContext::g_pTextureViews = WEngine::WArray<RHITextureView*>();

WEngine::WArray<RHITexture*> RHIContext::g_pDepthTextures = WEngine::WArray<RHITexture*>();

WEngine::WArray<RHITextureView*> RHIContext::g_pDepthTextureViews = WEngine::WArray<RHITextureView*>();

WEngine::WArray<RHICommandBuffer*> RHIContext::g_pCommandBuffers = WEngine::WArray<RHICommandBuffer*>();

WEngine::WArray<RHICommandBuffer*> RHIContext::g_pPrimaryCommandBuffers = WEngine::WArray<RHICommandBuffer*>();

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

	g_pPool = g_pQueue->GetCommandPool();

	g_pSurface = g_pInstance->GetSurface();

	RHISwapchainDescriptor swapchainDescriptor = {};
	{
		swapchainDescriptor.count = 3;
		swapchainDescriptor.format = Format::B8G8R8A8_UNorm;
		swapchainDescriptor.colorSpace = ColorSpace::SRGB_Linear;
		swapchainDescriptor.presenMode = PresentMode::Immediate;
		swapchainDescriptor.surface = g_pSurface;
		swapchainDescriptor.presentFamilyIndex = g_pQueue->GetIndex();
		swapchainDescriptor.extent = { Window::cur_window->GetWidth(), Window::cur_window->GetHeight() };
	}
	g_pSwapchain = g_pDevice->CreateSwapchain(&swapchainDescriptor);

	g_pTextureViews.Reserve(3);
	RHITextureViewDescriptor textureViewDescriptor = {};
	{
		textureViewDescriptor.format = Format::B8G8R8A8_UNorm;
		textureViewDescriptor.imageAspect = IMAGE_ASPECT_COLOR;
		textureViewDescriptor.mipCount = 1;
		textureViewDescriptor.baseMipLevel = 0;
		textureViewDescriptor.arrayLayerCount = 1;
		textureViewDescriptor.baseArrayLayer = 0;
		textureViewDescriptor.dimension = Dimension::Texture2D;
	}
	g_pTextureViews.Push(g_pSwapchain->GetTexture(0)->CreateTextureView(&textureViewDescriptor));
	g_pTextureViews.Push(g_pSwapchain->GetTexture(1)->CreateTextureView(&textureViewDescriptor));
	g_pTextureViews.Push(g_pSwapchain->GetTexture(2)->CreateTextureView(&textureViewDescriptor));

	g_pDepthTextures.Resize(3);
	g_pDepthTextureViews.Resize(3);
	RHITextureDescriptor depthTextureDescriptor = {};
	{
		depthTextureDescriptor.format = Format::D16_Unorm;
		depthTextureDescriptor.height = Window::cur_window->GetHeight();
		depthTextureDescriptor.width = Window::cur_window->GetWidth();
		depthTextureDescriptor.usage = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT;
		depthTextureDescriptor.mipCount = 1;
		depthTextureDescriptor.layout = AttachmentLayout::Undefined;
	}
	g_pDepthTextures[0] = g_pDevice->CreateTexture(&depthTextureDescriptor);
	g_pDepthTextures[1] = g_pDevice->CreateTexture(&depthTextureDescriptor);
	g_pDepthTextures[2] = g_pDevice->CreateTexture(&depthTextureDescriptor);

	for(int i = 0; i < 3; ++i)
	{
		TextureBarrier textureBarrier = { g_pDepthTextures[i], AttachmentLayout::Undefined, AttachmentLayout::DepthBuffer, 0, ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE, IMAGE_ASPECT_DEPTH };
		RHIBarrierDescriptor barrierDescriptor = {};
		{
			barrierDescriptor.textureCount = 1;
			barrierDescriptor.pTextureBarriers = &textureBarrier;
			barrierDescriptor.srcStage = PIPELINE_STAGE_TOP_OF_PIPE;
			barrierDescriptor.dstStage = PIPELINE_STAGE_EARLY_FRAGMENT_TESTS;
		}
		g_pContext->ResourceBarrier(&barrierDescriptor);
	}

	RHITextureViewDescriptor depthViewDescriptor = {};
	{
		depthViewDescriptor.format = Format::D16_Unorm;
		depthViewDescriptor.mipCount = 1;
		depthViewDescriptor.baseMipLevel = 0;
		depthViewDescriptor.arrayLayerCount = 1;
		depthViewDescriptor.baseArrayLayer = 0;
		depthViewDescriptor.dimension = Dimension::Texture2D;
		depthViewDescriptor.imageAspect = IMAGE_ASPECT_DEPTH;
	}
	for(int i = 0; i < 3; ++i)
		g_pDepthTextureViews[i] = g_pDepthTextures[i]->CreateTextureView(&depthViewDescriptor);

	g_pPrimaryCommandBuffers = g_pPool->GetCommandBuffer(3u, true);

	g_pFences = g_pDevice->CreateFence(g_maxFrames);
	g_pImageAvailibleSemaphores = g_pDevice->GetSemaphore(g_maxFrames);
	g_pPresentAVailibleSemaphores = g_pDevice->GetSemaphore(g_maxFrames);

	m_isDisplayChagned = false;
}

void RHIContext::RecreateSwapchain()
{
	g_pSwapchain->~RHISwapchain();
	WEngine::Allocator::Get()->Deallocate(g_pSwapchain);

	g_pInstance->UpdateSurface();

	RHISwapchainDescriptor swapchainDescriptor = {};
	{
		swapchainDescriptor.count = 3;
		swapchainDescriptor.format = Format::B8G8R8A8_UNorm;
		swapchainDescriptor.colorSpace = ColorSpace::SRGB_Linear;
		swapchainDescriptor.presenMode = PresentMode::Immediate;
		swapchainDescriptor.surface = g_pSurface;
		swapchainDescriptor.presentFamilyIndex = 0;
		swapchainDescriptor.extent = { Window::cur_window->GetWidth(), Window::cur_window->GetHeight() };
	}
	g_pSwapchain = g_pDevice->CreateSwapchain(&swapchainDescriptor);

	RHITextureViewDescriptor textureViewDescriptor = {};
	{
		textureViewDescriptor.format = Format::B8G8R8A8_UNorm;
		textureViewDescriptor.mipCount = 1;
		textureViewDescriptor.baseMipLevel = 0;
		textureViewDescriptor.arrayLayerCount = 1;
		textureViewDescriptor.baseArrayLayer = 0;
		textureViewDescriptor.dimension = Dimension::Texture2D;
		textureViewDescriptor.imageAspect = IMAGE_ASPECT_COLOR;
	}
	for (int i = 0; i < 3; ++i)
	{
		g_pTextureViews[i]->~RHITextureView();
		WEngine::Allocator::Get()->Deallocate(g_pTextureViews[i]);
		g_pTextureViews[i] = g_pSwapchain->GetTexture(i)->CreateTextureView(&textureViewDescriptor);
	}

	m_isDisplayChagned = true;
}

RHITexture* RHIContext::GetTexture(unsigned int index)
{
	return g_pSwapchain->GetTexture(index);
}

RHITextureView* RHIContext::GetTextureView(unsigned int index)
{
	return g_pTextureViews[index];
}

RHITextureView* RHIContext::GetDepthView(unsigned int index)
{
	return g_pDepthTextureViews[index];
}

RHICommandBuffer* RHIContext::GetCommandBuffer(bool isPrimary)
{
	return g_pPool->GetCommandBuffer(isPrimary);
}

WEngine::WArray<RHICommandBuffer*> RHIContext::GetCommandBuffer(unsigned int count, bool isPrimary)
{
	return g_pPool->GetCommandBuffer(count, isPrimary);
}

int RHIContext::GetNextImage(RHISemaphore *pSignalSemaphore)
{
	return g_pDevice->GetNextImage(g_pSwapchain, pSignalSemaphore);
}

void RHIContext::ExecuteCommandBuffer(RHICommandBuffer* cmd)
{
	g_pCommandBuffers.Push(cmd);
}

void RHIContext::Submit(RHISubmitDescriptor* descriptor)
{
	g_pPrimaryCommandBuffers[g_currentFrame]->Clear();
	g_pPrimaryCommandBuffers[g_currentFrame]->BeginScopePass("Frame");
	for(unsigned int i = 0; i < g_pCommandBuffers.Size(); ++i)
	{
		g_pPrimaryCommandBuffers[g_currentFrame]->ExecuteCommandBuffer(g_pCommandBuffers[i]);
	}
	g_pPrimaryCommandBuffers[g_currentFrame]->EndScopePass();

	descriptor->commandBufferCount = 1;
	descriptor->pCommandBuffers = &g_pPrimaryCommandBuffers[g_currentFrame];
	g_pQueue->Submit(descriptor);
	g_pCommandBuffers.Clear();
}

int RHIContext::GetNextImage()
{
	g_pDevice->WaitForFences(g_pFences[g_currentFrame], 1);

	g_currentImage = GetNextImage(g_pImageAvailibleSemaphores[g_currentFrame]);
	if (g_currentImage < 0)
	{
		g_pContext->RecreateSwapchain();
		return -1;
	}
	g_pDevice->ResetFences(g_pFences[g_currentFrame], 1);

	return g_currentImage;
}

void RHIContext::Present(unsigned int imageIndex)
{
	if (!g_pQueue->Present(g_pSwapchain, imageIndex, g_pPresentAVailibleSemaphores[g_currentFrame]))
	{
		g_pContext->RecreateSwapchain();
	}

	g_currentFrame = (g_currentFrame + 1) % g_maxFrames;
}

bool RHIContext::IsDisplayChanged()
{
	return m_isDisplayChagned;
}

void RHIContext::ResetDisplayState()
{
	m_isDisplayChagned = false;
}

void RHIContext::ResourceBarrier(RHIBarrierDescriptor *pDescriptor)
{
	RHICommandBuffer *cmd = g_pPool->GetCommandBuffer(true);
	
	cmd->BeginScopePass("Layout Transition");
	{
		RHIGraphicsEncoder *encoder = cmd->GetGraphicsEncoder();
		encoder->ResourceBarrier(pDescriptor);
		encoder->~RHIGraphicsEncoder();
		WEngine::Allocator::Get()->Deallocate(encoder);
	}
	cmd->EndScopePass();
	RHISubmitDescriptor submitDescriptor = {};
	{
		submitDescriptor.pFence = nullptr;
		submitDescriptor.waitSemaphoreCount = 0;
		submitDescriptor.pWaitSemaphores = nullptr;
		submitDescriptor.signalSemaphoreCount = 0;
		submitDescriptor.pSignalSemaphores = nullptr;
		submitDescriptor.waitStage = PIPELINE_STAGE_ALL_COMMANDS;
		submitDescriptor.commandBufferCount = 1;
		submitDescriptor.pCommandBuffers = &cmd;
	}
	g_pQueue->Submit(&submitDescriptor);
	g_pDevice->Wait();
	cmd->~RHICommandBuffer();
	WEngine::Allocator::Get()->Deallocate(cmd);
}

RHIBuffer* RHIContext::CreateVertexBuffer(RHIBufferDescriptor* descriptor)
{
	descriptor->bufferType = BUFFER_USAGE_VERTEX_BUFFER;
	return g_pDevice->CreateBuffer(descriptor);
}

RHIBuffer* RHIContext::CreateIndexBuffer(RHIBufferDescriptor* descriptor)
{
	descriptor->bufferType = BUFFER_USAGE_INDEX_BUFFER;
	return g_pDevice->CreateBuffer(descriptor);
}

RHIBuffer* RHIContext::CreateUniformBuffer(RHIBufferDescriptor* descriptor)
{
	descriptor->bufferType = BUFFER_USAGE_UNIFORM_BUFFER;
	return g_pDevice->CreateBuffer(descriptor);
}

RHIBuffer* RHIContext::CreateTextureBuffer(RHIBufferDescriptor* descriptor)
{
	descriptor->bufferType = BUFFER_USAGE_TRANSFER_SRC;
	return g_pDevice->CreateBuffer(descriptor);
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

	RHICommandBuffer *cmd = GetCommandBuffer(true);
	cmd->BeginScopePass("Copy Buffer");
	{
		RHIGraphicsEncoder *encoder = cmd->GetGraphicsEncoder();
		TextureBarrier textureBarrier = { pTexture, AttachmentLayout::Undefined, AttachmentLayout::BlitDst, 0, ACCESS_TRANSFER_WRITE, IMAGE_ASPECT_COLOR };
		RHIBarrierDescriptor barrierDescriptor = {};
		{
			barrierDescriptor.srcStage = PIPELINE_STAGE_HOST;
			barrierDescriptor.dstStage = PIPELINE_STAGE_TRANSFER;
			barrierDescriptor.textureCount = 1;
			barrierDescriptor.pTextureBarriers = &textureBarrier;
		}
		encoder->ResourceBarrier(&barrierDescriptor);
		encoder->CopyBufferToImage(pTexture, pBuffer, width, height);
		{
			barrierDescriptor.srcStage = PIPELINE_STAGE_TRANSFER;
			barrierDescriptor.dstStage = PIPELINE_STAGE_FRAGMENT_SHADER;
			textureBarrier.srcAccess = ACCESS_TRANSFER_WRITE;
			textureBarrier.dstAccess = ACCESS_SHADER_READ;
			textureBarrier.oldLayout = AttachmentLayout::BlitDst;
			textureBarrier.newLayout = AttachmentLayout::ReadOnlyColor;
		}
		encoder->ResourceBarrier(&barrierDescriptor);
	}
	cmd->EndScopePass();
	RHISubmitDescriptor submitDescriptor = {};
	{
		submitDescriptor.pFence = nullptr;
		submitDescriptor.waitSemaphoreCount = 0;
		submitDescriptor.pWaitSemaphores = nullptr;
		submitDescriptor.signalSemaphoreCount = 0;
		submitDescriptor.pSignalSemaphores = nullptr;
		submitDescriptor.waitStage = 0;
		submitDescriptor.commandBufferCount = 1;
		submitDescriptor.pCommandBuffers = &cmd;
	}
	g_pQueue->Submit(&submitDescriptor);
	g_pDevice->Wait();
	cmd->~RHICommandBuffer();
	WEngine::Allocator::Get()->Deallocate(cmd);

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