#include "pch.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Render/Public/ScriptableRenderer.h"
#include "Render/Passes/Public/RenderPassHeads.h"
#include "RHI/Public/RHIHeads.h"
#include "RHI/Public/RHIGPU.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Platform/Vulkan/Public/VulkanInstance.h"
#include "Utils/Public/Window.h"
#include "Scene/Public/World.h"
#include "Scene/Components/Public/Camera.h"
#include "Utils/ImGui/Public/Gui.h"

ScriptableRenderPipeline *ScriptableRenderPipeline::g_instance = nullptr;

int ScriptableRenderPipeline::g_currentImage = 0;

unsigned int ScriptableRenderPipeline::g_currentFrame = 0;

unsigned int ScriptableRenderPipeline::g_maxFrame = 3;

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
		descriptor.enableGPUValidator = true;
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

	m_pContext = new RHIContext(queue, m_pInstance->GetSurface(), m_pDevice);

	m_pContext->Init();

	m_pImageAvailibleSemaphores = m_pDevice->GetSemaphore(g_maxFrame);
	m_pPresentAVailibleSemaphores = m_pDevice->GetSemaphore(g_maxFrame);

	m_pFences = m_pDevice->CreateFence(g_maxFrame);

	GuiConfigure guiConfigure = {};
	{
		guiConfigure.pInstance = m_pInstance;
		guiConfigure.pGPU = m_pInstance->GetGPU(0);
		guiConfigure.pDevice = m_pDevice;
		guiConfigure.pQueue = queue;
		guiConfigure.imageCount = 3;
	}

	Gui* pGui = Gui::CreateGui(WEngine::Backend::Vulkan);
	pGui->Init(&guiConfigure);

	m_pCameras = World::GetWorld()->GetCameras();
}

void ScriptableRenderPipeline::Setup()
{
	m_pCameras = World::GetWorld()->GetCameras();
	m_pCameraDatas.reserve(m_pCameras.size());

	RenderPassConfigure configure =
	{
		m_pDevice
	};

	for (Camera *camera : m_pCameras)
	{
		CameraData *data = camera->GetData();
		ScriptableRenderer* renderer = camera->GetRenderer();
		//renderer->EnqueRenderPass(new DrawOpaquePass(&configure));
		//renderer->EnqueRenderPass(new DrawSkyboxPass(&configure));
		renderer->EnqueRenderPass(new DrawGUIPass(&configure));
		renderer->Setup(m_pContext, data);
		m_pCameraDatas.push_back(data);
	}
}

void ScriptableRenderPipeline::Execute()
{
	m_pDevice->WaitForFences(m_pFences[g_currentFrame], 1);

	g_currentImage = m_pContext->GetNextImage(m_pImageAvailibleSemaphores[g_currentFrame]);
	if (g_currentImage < 0)
	{
		m_pInstance->UpdateSurface();
		m_pContext->RecreateSwapchain();
		return;
	}
	m_pDevice->ResetFences(m_pFences[g_currentFrame], 1);

	for (Camera *camera : m_pCameras)
	{
		RenderSingleCamera(camera, m_pContext, m_pImageAvailibleSemaphores[g_currentFrame], m_pPresentAVailibleSemaphores[g_currentFrame], m_pFences[g_currentFrame]);
	}

	if (!m_pContext->Present(g_currentFrame, m_pPresentAVailibleSemaphores[g_currentFrame]))
	{
		m_pInstance->UpdateSurface();
		m_pContext->RecreateSwapchain();
	}

	g_currentFrame = (g_currentFrame + 1) % g_maxFrame;
}

ScriptableRenderer* ScriptableRenderPipeline::CreateRenderer()
{
	RendererConfigure configure = { m_pDevice, m_pContext };

	ScriptableRenderer *renderer = new ScriptableRenderer(&configure);
	
	return renderer;
}

void ScriptableRenderPipeline::RenderSingleCamera(Camera* camera, RHIContext* context, RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence* fence)
{
	CameraData *data = camera->GetData();
	camera->GetRenderer()->Execute(context, data, waitSemaphore, signalSemaphore, fence);
}
