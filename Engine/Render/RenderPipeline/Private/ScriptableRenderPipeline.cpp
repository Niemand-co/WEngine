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
#include "Render/Descriptor/Public/RHIInstanceDescriptor.h"
#include "Render/Descriptor/Public/RHIDeviceDescriptor.h"
#include "Render/Descriptor/Public/RHIQueueDescriptor.h"
#include "Render/Descriptor/Public/RHISwapchainDescriptor.h"
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
		g_instance = new ScriptableRenderPipeline();
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
	deviceDescriptor.queueInfos = queueDescriptors.data();
	deviceDescriptor.queueInfoCount = queueDescriptors.size();

	m_pDevice = m_pInstance->GetGPU(0)->CreateDevice(&deviceDescriptor);

	RHIQueue* queue = m_pDevice->GetQueue(RHIQueueType::Graphics, 1);

	RHIContext* context = new RHIContext(queue, m_pDevice);

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

	m_pContext = new RHIContext(queue, m_pDevice);

	m_semaphoreCount = 2;
	m_semaphores = m_pDevice->GetSemaphore(m_semaphoreCount);
}

void ScriptableRenderPipeline::Setup()
{
}

void ScriptableRenderPipeline::Execute()
{
	g_currentFrame = m_pDevice->GetNextImage(m_pSwapchain, m_semaphores[0]);
	g_currentRenderTarget = m_pSwapchain->GetTexture(g_currentFrame);
	if(g_currentFrame < 0 || Window::cur_window->GetIsClosed())
		return;

	for (ScriptableRenderer* renderer : m_renderers)
	{
		renderer->Execute(m_semaphores[0], m_semaphores[1]);
	}

	m_pContext->Present(g_currentFrame, m_pSwapchain, m_semaphores[1]);
}

void ScriptableRenderPipeline::AddRenderer()
{
	RendererConfigure configure = { m_pDevice, m_pContext };

	ScriptableRenderer *renderer = (ScriptableRenderer*)Allocator::Allocate(sizeof(ScriptableRenderer));
	::new (renderer) ScriptableRenderer(&configure);
	m_renderers.push_back(renderer);

	renderer->AddRenderPass();
}

void ScriptableRenderPipeline::Present()
{
}
