#include "pch.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Render/Public/ScriptableRenderer.h"
#include "RHI/Public/RHIInstance.h"
#include "RHI/Public/RHIGPU.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIQueue.h"
#include "RHI/Public/RHISwapchain.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHITexture.h"
#include "RHI/Public/RHISemaphore.h"
#include "RHI/Public/RHIFence.h"
#include "Render/Descriptor/Public/RHIInstanceDescriptor.h"
#include "Render/Descriptor/Public/RHIDeviceDescriptor.h"
#include "Render/Descriptor/Public/RHIQueueDescriptor.h"
#include "Render/Descriptor/Public/RHISwapchainDescriptor.h"
#include "Platform/Vulkan/Public/VulkanInstance.h"
#include "Utils/Public/Window.h"

ScriptableRenderPipeline *ScriptableRenderPipeline::g_instance = nullptr;

int ScriptableRenderPipeline::g_currentFrame = 0;

RHITexture *ScriptableRenderPipeline::g_currentRenderTarget = nullptr;

ScriptableRenderPipeline::ScriptableRenderPipeline()
{
	Init();
}

ScriptableRenderPipeline::~ScriptableRenderPipeline()
{
}

ScriptableRenderPipeline* ScriptableRenderPipeline::get()
{
	if (g_instance == nullptr)
	{
		g_instance = (ScriptableRenderPipeline*)WEngine::Allocator::Get()->Allocate(sizeof(ScriptableRenderPipeline));
		::new (g_instance) ScriptableRenderPipeline();
	}
	return g_instance;
}

void ScriptableRenderPipeline::Init()
{
	RHIInstanceDescriptor descriptor = {};
	{
		descriptor.backend = RHIBackend::Vulkan;
		descriptor.enableDebugLayer = true;
		descriptor.enableGPUValidator = false;
	}
	m_pInstance = RHIInstance::CreateInstance(&descriptor);

	std::vector<RHIQueueDescriptor> queueDescriptors(1, RHIQueueDescriptor());
	{
		queueDescriptors[0].count = 1;
		queueDescriptors[0].type = RHIQueueType::Graphics;
	}
	RHIDeviceDescriptor deviceDescriptor;
	{
		deviceDescriptor.queueInfos = queueDescriptors.data();
		deviceDescriptor.queueInfoCount = queueDescriptors.size();
	}

	m_pDevice = m_pInstance->GetGPU(0)->CreateDevice(&deviceDescriptor);

	RHIQueue* queue = m_pDevice->GetQueue(RHIQueueType::Graphics, 1);

	RHIContext* context = (RHIContext*)WEngine::Allocator::Get()->Allocate(sizeof(RHIContext));
	::new (context) RHIContext(queue, m_pDevice);

	RHISwapchainDescriptor swapchainDescriptor = {};
	{
		swapchainDescriptor.count = 3;
		swapchainDescriptor.format = Format::A16R16G16B16_SFloat;
		swapchainDescriptor.colorSpace = ColorSpace::SRGB_Linear;
		swapchainDescriptor.presenMode = PresentMode::Immediate;
		swapchainDescriptor.surface = m_pInstance->GetSurface();
		swapchainDescriptor.presentFamilyIndex = queue->GetIndex();
		swapchainDescriptor.extent = { Window::cur_window->GetWidth(), Window::cur_window->GetHeight() };
	}
	m_pSwapchain = m_pDevice->CreateSwapchain(&swapchainDescriptor);

	m_maxFrame = 3;
	m_currentFrame = 0;
	m_pImageAvailibleSemaphores = m_pDevice->GetSemaphore(m_maxFrame);
	m_pPresentAVailibleSemaphores = m_pDevice->GetSemaphore(m_maxFrame);

	m_pFences = m_pDevice->CreateFence(m_maxFrame);
}

void ScriptableRenderPipeline::Setup()
{
	for (ScriptableRenderer* renderer : m_renderers)
	{
		renderer->Setup();
	}
}

void ScriptableRenderPipeline::Execute()
{
	//m_pDevice->WaitForFences(m_fences[m_currentFrame], 1);
	//m_pDevice->ResetFences(m_fences[m_currentFrame], 1);

	g_currentFrame = m_pDevice->GetNextImage(m_pSwapchain, m_pImageAvailibleSemaphores + m_currentFrame);
	if (g_currentFrame < 0)
	{
		m_pInstance->UpdateSurface();
		RHISwapchainDescriptor swapchainDescriptor = {};
		{
			swapchainDescriptor.count = 3;
			swapchainDescriptor.format = Format::A16R16G16B16_SFloat;
			swapchainDescriptor.colorSpace = ColorSpace::SRGB_Linear;
			swapchainDescriptor.presenMode = PresentMode::Immediate;
			swapchainDescriptor.surface = m_pInstance->GetSurface();
			swapchainDescriptor.presentFamilyIndex = 0;
			swapchainDescriptor.extent = { Window::cur_window->GetWidth(), Window::cur_window->GetHeight() };
		}
		m_pDevice->RecreateSwapchain(m_pSwapchain, &swapchainDescriptor);
		return;
	}
	//g_currentRenderTarget = m_pSwapchain->GetTexture(g_currentFrame);

	//for (ScriptableRenderer* renderer : m_renderers)
	//{
	//	renderer->Execute(m_imageAvailibleSemaphores[m_currentFrame], m_presentAVailibleSemaphores[m_currentFrame], m_fences[m_currentFrame]);
	//}

	if (!m_pContext->Present(g_currentFrame, m_pSwapchain, m_pImageAvailibleSemaphores + m_currentFrame))
	{
		m_pInstance->UpdateSurface();
		RHISwapchainDescriptor swapchainDescriptor = {};
		{
			swapchainDescriptor.count = 3;
			swapchainDescriptor.format = Format::A16R16G16B16_SFloat;
			swapchainDescriptor.colorSpace = ColorSpace::SRGB_Linear;
			swapchainDescriptor.presenMode = PresentMode::Immediate;
			swapchainDescriptor.surface = m_pInstance->GetSurface();
			swapchainDescriptor.presentFamilyIndex = 0;
			swapchainDescriptor.extent = { Window::cur_window->GetWidth(), Window::cur_window->GetHeight() };
		}
		m_pDevice->RecreateSwapchain(m_pSwapchain, &swapchainDescriptor);
		return;
	}

	m_currentFrame = (m_currentFrame + 1) % m_maxFrame;
}

void ScriptableRenderPipeline::AddRenderer()
{
	RendererConfigure configure = { m_pDevice, m_pContext };

	ScriptableRenderer *renderer = (ScriptableRenderer*)WEngine::Allocator::Get()->Allocate(sizeof(ScriptableRenderer));
	::new (renderer) ScriptableRenderer(&configure);
	m_renderers.push_back(renderer);
}

void ScriptableRenderPipeline::Present()
{
}
