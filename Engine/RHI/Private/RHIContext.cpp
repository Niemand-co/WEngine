#include "pch.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHICommandPool.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "RHI/Public/RHIQueue.h"
#include "RHI/Public/RHISwapchain.h"
#include "RHI/Public/RHISurface.h"
#include "RHI/Public/RHITexture.h"
#include "RHI/Public/RHITextureView.h"
#include "RHI/Public/RHISemaphore.h"
#include "Render/Descriptor/Public/RHISwapchainDescriptor.h"
#include "Render/Descriptor/Public/RHIInstanceDescriptor.h"
#include "Render/Descriptor/Public/RHISwapchainDescriptor.h"
#include "Render/Descriptor/Public/RHITextureViewDescriptor.h"
#include "Utils/Public/Window.h"

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
	m_pTextureViews.push_back(m_pSwapchain->GetTexture(0)->CreateTextureView(&textureViewDescriptor));
	m_pTextureViews.push_back(m_pSwapchain->GetTexture(1)->CreateTextureView(&textureViewDescriptor));
	m_pTextureViews.push_back(m_pSwapchain->GetTexture(2)->CreateTextureView(&textureViewDescriptor));

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

RHITextureView* RHIContext::GetTextureView(unsigned int index)
{
	return m_pTextureViews[index];
}

RHICommandBuffer* RHIContext::GetCommandBuffer()
{
	return m_pPool->GetCommandBuffer();
}

int RHIContext::GetNextImage(RHISemaphore *pSignalSemaphore)
{
	return m_pDevice->GetNextImage(m_pSwapchain, pSignalSemaphore);
}

void RHIContext::ExecuteCommandBuffer(RHICommandBuffer* cmd)
{
	m_pCommandBuffers.push_back(cmd);
}

void RHIContext::Submit(RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence* fence)
{
	m_pQueue->Submit(m_pCommandBuffers.data(), m_pCommandBuffers.size(), waitSemaphore, signalSemaphore, fence);
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
