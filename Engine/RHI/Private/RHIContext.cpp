#include "pch.h"
#include "RHI/Public/RHIHeads.h"
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
		textureDescriptor.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		textureDescriptor.mipCount = 1;
		textureDescriptor.layout = AttachmentLayout::ColorBuffer;
	}
	m_pTextureViews.push_back(m_pSwapchain->GetTexture(0)->CreateTextureView(&textureViewDescriptor));
	m_pTextureViews.push_back(m_pSwapchain->GetTexture(1)->CreateTextureView(&textureViewDescriptor));
	m_pTextureViews.push_back(m_pSwapchain->GetTexture(2)->CreateTextureView(&textureViewDescriptor));

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

void RHIContext::UpdateResourceToGroup(RHIUpdateResourceDescriptor* descriptor)
{
	m_pDevice->UpdateResourceToGroup(descriptor);
}

RHIPipelineResourceLayout* RHIContext::CreatePipelineResourceLayout(RHIPipelineResourceLayoutDescriptor* descriptor)
{
	return m_pDevice->CreatePipelineResourceLayout(descriptor);
}

RHIPipelineStateObject* RHIContext::CreatePSO(RHIPipelineStateObjectDescriptor* descriptor)
{
	return m_pDevice->CreatePipelineStateObject(descriptor);
}
