#include "pch.h"
#include "RHI/Public/RHIHeads.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Utils/Public/Window.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

RHIContext::RHIContext(RHIQueue *queue, RHISurface *surface, RHIDevice *device)
	: m_pQueue(queue), m_pDevice(device), m_pSurface(surface)
{
	m_pPool = queue->GetCommandPool();
}

void RHIContext::Init()
{
	RHISwapchainDescriptor swapchainDescriptor = {};
	{
		swapchainDescriptor.count = 3;
		swapchainDescriptor.format = Format::A16R16G16B16_SFloat;
		swapchainDescriptor.colorSpace = ColorSpace::SRGB_Linear;
		swapchainDescriptor.presenMode = PresentMode::Immediate;
		swapchainDescriptor.surface = m_pSurface;
		swapchainDescriptor.presentFamilyIndex = m_pQueue->GetIndex();
		swapchainDescriptor.extent = { Window::cur_window->GetWidth(), Window::cur_window->GetHeight() };
	}
	m_pSwapchain = m_pDevice->CreateSwapchain(&swapchainDescriptor);

	m_pTextureViews.reserve(3);
	RHITextureViewDescriptor textureViewDescriptor = {};
	{
		textureViewDescriptor.format = Format::A16R16G16B16_SFloat;
		textureViewDescriptor.imageAspect = IMAGE_ASPECT_COLOR;
		textureViewDescriptor.mipCount = 1;
		textureViewDescriptor.baseMipLevel = 0;
		textureViewDescriptor.arrayLayerCount = 1;
		textureViewDescriptor.baseArrayLayer = 0;
		textureViewDescriptor.dimension = Dimension::Texture2D;
	}
	RHITextureDescriptor textureDescriptor = {};
	{
		textureDescriptor.format = Format::A16R16G16B16_SFloat;
		textureDescriptor.width = 1920;
		textureDescriptor.height = 1080;
		textureDescriptor.usage = IMAGE_USAGE_COLOR_ATTACHMENT;
		textureDescriptor.mipCount = 1;
		textureDescriptor.layout = AttachmentLayout::ColorBuffer;
	}
	m_pTextureViews.push_back(m_pSwapchain->GetTexture(0)->CreateTextureView(&textureViewDescriptor));
	m_pTextureViews.push_back(m_pSwapchain->GetTexture(1)->CreateTextureView(&textureViewDescriptor));
	m_pTextureViews.push_back(m_pSwapchain->GetTexture(2)->CreateTextureView(&textureViewDescriptor));

	m_pDepthTextures.resize(3);
	m_pDepthTextureViews.resize(3);
	RHITextureDescriptor depthTextureDescriptor = {};
	{
		depthTextureDescriptor.format = Format::D16_Unorm;
		depthTextureDescriptor.height = Window::cur_window->GetHeight();
		depthTextureDescriptor.width = Window::cur_window->GetWidth();
		depthTextureDescriptor.usage = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT;
		depthTextureDescriptor.mipCount = 1;
		depthTextureDescriptor.layout = AttachmentLayout::Undefined;
	}
	m_pDepthTextures[0] = m_pDevice->CreateTexture(&depthTextureDescriptor);
	m_pDepthTextures[1] = m_pDevice->CreateTexture(&depthTextureDescriptor);
	m_pDepthTextures[2] = m_pDevice->CreateTexture(&depthTextureDescriptor);

	for(int i = 0; i < 3; ++i)
	{
		TextureBarrier textureBarrier = { m_pDepthTextures[i], AttachmentLayout::Undefined, AttachmentLayout::DepthBuffer, 0, ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE, IMAGE_ASPECT_DEPTH };
		RHIBarrierDescriptor barrierDescriptor = {};
		{
			barrierDescriptor.textureCount = 1;
			barrierDescriptor.pTextureBarriers = &textureBarrier;
			barrierDescriptor.srcStage = PIPELINE_STAGE_TOP_OF_PIPE;
			barrierDescriptor.dstStage = PIPELINE_STAGE_EARLY_FRAGMENT_TESTS;
		}
		ResourceBarrier(&barrierDescriptor);
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
		m_pDepthTextureViews[i] = m_pDepthTextures[i]->CreateTextureView(&depthViewDescriptor);

	m_pPrimaryCommandBuffers = m_pPool->GetCommandBuffer(3u, true);

	m_isDisplayChagned = false;
}

void RHIContext::RecreateSwapchain()
{
	m_pSwapchain->~RHISwapchain();
	WEngine::Allocator::Get()->Deallocate(m_pSwapchain);

	RHISwapchainDescriptor swapchainDescriptor = {};
	{
		swapchainDescriptor.count = 3;
		swapchainDescriptor.format = Format::A16R16G16B16_SFloat;
		swapchainDescriptor.colorSpace = ColorSpace::SRGB_Linear;
		swapchainDescriptor.presenMode = PresentMode::Immediate;
		swapchainDescriptor.surface = m_pSurface;
		swapchainDescriptor.presentFamilyIndex = 0;
		swapchainDescriptor.extent = { Window::cur_window->GetWidth(), Window::cur_window->GetHeight() };
	}
	m_pSwapchain = m_pDevice->CreateSwapchain(&swapchainDescriptor);

	RHITextureViewDescriptor textureViewDescriptor = {};
	{
		textureViewDescriptor.format = Format::A16R16G16B16_SFloat;
		textureViewDescriptor.mipCount = 1;
		textureViewDescriptor.baseMipLevel = 0;
		textureViewDescriptor.arrayLayerCount = 1;
		textureViewDescriptor.baseArrayLayer = 0;
		textureViewDescriptor.dimension = Dimension::Texture2D;
	}
	for (int i = 0; i < 3; ++i)
	{
		m_pTextureViews[i]->~RHITextureView();
		WEngine::Allocator::Get()->Deallocate(m_pTextureViews[i]);
		m_pTextureViews[i] = m_pSwapchain->GetTexture(i)->CreateTextureView(&textureViewDescriptor);
	}

	m_isDisplayChagned = true;
}

RHITexture* RHIContext::GetTexture(unsigned int index)
{
	return m_pSwapchain->GetTexture(index);
}

RHITextureView* RHIContext::GetTextureView(unsigned int index)
{
	return m_pTextureViews[index];
}

RHITextureView* RHIContext::GetDepthView(unsigned int index)
{
	return m_pDepthTextureViews[index];
}

RHICommandBuffer* RHIContext::GetCommandBuffer(bool isPrimary)
{
	return m_pPool->GetCommandBuffer(isPrimary);
}

std::vector<RHICommandBuffer*> RHIContext::GetCommandBuffer(unsigned int count, bool isPrimary)
{
	return m_pPool->GetCommandBuffer(count, isPrimary);
}

int RHIContext::GetNextImage(RHISemaphore *pSignalSemaphore)
{
	return m_pDevice->GetNextImage(m_pSwapchain, pSignalSemaphore);
}

void RHIContext::ExecuteCommandBuffer(RHICommandBuffer* cmd)
{
	m_pCommandBuffers.push_back(cmd);
}

void RHIContext::Submit(RHISubmitDescriptor* descriptor)
{
	m_pPrimaryCommandBuffers[ScriptableRenderPipeline::g_currentFrame]->Clear();
	m_pPrimaryCommandBuffers[ScriptableRenderPipeline::g_currentFrame]->BeginScopePass("Frame");
	for(unsigned int i = 0; i < m_pCommandBuffers.size(); ++i)
	{
		m_pPrimaryCommandBuffers[ScriptableRenderPipeline::g_currentFrame]->ExecuteCommandBuffer(m_pCommandBuffers[i]);
	}
	m_pPrimaryCommandBuffers[ScriptableRenderPipeline::g_currentFrame]->EndScopePass();

	descriptor->commandBufferCount = 1;
	descriptor->pCommandBuffers = &m_pPrimaryCommandBuffers[ScriptableRenderPipeline::g_currentFrame];
	m_pQueue->Submit(descriptor);
	m_pCommandBuffers.clear();
}

bool RHIContext::Present(unsigned int imageIndex, RHISemaphore *semaphore)
{
	return m_pQueue->Present(m_pSwapchain, imageIndex, semaphore);
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
	RHICommandBuffer *cmd = m_pPool->GetCommandBuffer(true);
	
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
	m_pQueue->Submit(&submitDescriptor);
	m_pDevice->Wait();
	cmd->~RHICommandBuffer();
	WEngine::Allocator::Get()->Deallocate(cmd);
}

RHIBuffer* RHIContext::CreateVertexBuffer(RHIBufferDescriptor* descriptor)
{
	descriptor->bufferType = BUFFER_USAGE_VERTEX_BUFFER;
	return m_pDevice->CreateBuffer(descriptor);
}

RHIBuffer* RHIContext::CreateIndexBuffer(RHIBufferDescriptor* descriptor)
{
	descriptor->bufferType = BUFFER_USAGE_INDEX_BUFFER;
	return m_pDevice->CreateBuffer(descriptor);
}

RHIBuffer* RHIContext::CreateUniformBuffer(RHIBufferDescriptor* descriptor)
{
	descriptor->bufferType = BUFFER_USAGE_UNIFORM_BUFFER;
	return m_pDevice->CreateBuffer(descriptor);
}

RHIBuffer* RHIContext::CreateTextureBuffer(RHIBufferDescriptor* descriptor)
{
	descriptor->bufferType = BUFFER_USAGE_TRANSFER_SRC;
	return m_pDevice->CreateBuffer(descriptor);
}

void RHIContext::CopyBufferToImage(RHITexture* pTexture, RHIBuffer* pBuffer, unsigned int width, unsigned int height)
{

	RHICommandBuffer *cmd = GetCommandBuffer(false);
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
	m_pQueue->Submit(&submitDescriptor);
	m_pDevice->Wait();
	cmd->~RHICommandBuffer();
	WEngine::Allocator::Get()->Deallocate(cmd);

}

RHIGroupLayout* RHIContext::CreateGroupLayout(RHIGroupLayoutDescriptor* descriptor)
{
	return m_pDevice->CreateGroupLayout(descriptor);
}

RHIGroup* RHIContext::CreateResourceGroup(RHIGroupDescriptor* descriptor)
{
	RHIGroupPoolDescriptor poolDescriptor = {};
	{
		poolDescriptor.pGroupLayout = descriptor->pGroupLayout;
		poolDescriptor.maxSetCount = 3;
	}
	RHIGroupPool *pool = m_pDevice->CreateGroupPool(&poolDescriptor);

	RHIGroup *group = pool->GetGroup();

	return group;
}

void RHIContext::UpdateUniformResourceToGroup(RHIUpdateResourceDescriptor* descriptor)
{
	m_pDevice->UpdateUniformResourceToGroup(descriptor);
}

void RHIContext::UpdateTextureResourceToGroup(RHIUpdateResourceDescriptor* descriptor)
{
	m_pDevice->UpdateTextureResourceToGroup(descriptor);
}

RHIPipelineResourceLayout* RHIContext::CreatePipelineResourceLayout(RHIPipelineResourceLayoutDescriptor* descriptor)
{
	return m_pDevice->CreatePipelineResourceLayout(descriptor);
}

RHIPipelineStateObject* RHIContext::CreatePSO(RHIPipelineStateObjectDescriptor* descriptor)
{
	return m_pDevice->CreatePipelineStateObject(descriptor);
}
