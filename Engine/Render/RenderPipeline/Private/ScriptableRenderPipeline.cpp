#include "pch.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Render/Public/ScriptableRenderer.h"
#include "RHI/Public/RHIInstance.h"
#include "RHI/Public/RHIGPU.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIQueue.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHITexture.h"
#include "RHI/Public/RHISemaphore.h"
#include "RHI/Public/RHIFence.h"
#include "Render/Descriptor/Public/RHIInstanceDescriptor.h"
#include "Render/Descriptor/Public/RHIDeviceDescriptor.h"
#include "Render/Descriptor/Public/RHIQueueDescriptor.h"
#include "Platform/Vulkan/Public/VulkanInstance.h"
#include "Utils/Public/Window.h"
#include "Scene/Public/World.h"
#include "Scene/Components/Public/Camera.h"
#include "Utils/ImGui/Public/Gui.h"

ScriptableRenderPipeline *ScriptableRenderPipeline::g_instance = nullptr;

int ScriptableRenderPipeline::g_currentFrame = 0;

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

	m_pContext = (RHIContext*)WEngine::Allocator::Get()->Allocate(sizeof(RHIContext));
	::new (m_pContext) RHIContext(queue, m_pInstance->GetSurface(), m_pDevice);

	m_pContext->Init();

	m_maxFrame = 3;
	m_currentFrame = 0;
	m_pImageAvailibleSemaphores = m_pDevice->GetSemaphore(m_maxFrame);
	m_pPresentAVailibleSemaphores = m_pDevice->GetSemaphore(m_maxFrame);

	m_pFences = m_pDevice->CreateFence(m_maxFrame);

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
}

void ScriptableRenderPipeline::Setup()
{
	m_pCameras = World::GetWorld()->GetCameras();
	m_pCameraDatas.reserve(m_pCameras.size());

	for (Camera *camera : m_pCameras)
	{
		CameraData *data = camera->GetData();
		camera->GetRenderer()->Setup(data);
		m_pCameraDatas.push_back(data);
	}
}

void ScriptableRenderPipeline::Execute()
{
	m_pDevice->WaitForFences(m_pFences[m_currentFrame], 1);

	g_currentFrame = m_pContext->GetNextImage(m_pImageAvailibleSemaphores[m_currentFrame]);
	if (g_currentFrame < 0)
	{
		m_pInstance->UpdateSurface();
		m_pContext->RecreateSwapchain();
		return;
	}
	m_pDevice->ResetFences(m_pFences[m_currentFrame], 1);

	for (Camera *camera : m_pCameras)
	{
		RenderSingleCamera(camera, m_pContext, m_pImageAvailibleSemaphores[m_currentFrame], m_pPresentAVailibleSemaphores[m_currentFrame], m_pFences[m_currentFrame]);
	}

	if (!m_pContext->Present(g_currentFrame, m_pPresentAVailibleSemaphores[m_currentFrame]))
	{
		m_pInstance->UpdateSurface();
		m_pContext->RecreateSwapchain();
	}

	m_currentFrame = (m_currentFrame + 1) % m_maxFrame;
}

ScriptableRenderer* ScriptableRenderPipeline::CreateRenderer()
{
	RendererConfigure configure = { m_pDevice, m_pContext };

	ScriptableRenderer *renderer = (ScriptableRenderer*)WEngine::Allocator::Get()->Allocate(sizeof(ScriptableRenderer));
	::new (renderer) ScriptableRenderer(&configure);
	
	return renderer;
}

void ScriptableRenderPipeline::RenderSingleCamera(Camera* camera, RHIContext* context, RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence* fence)
{
	camera->GetRenderer()->Execute(context, waitSemaphore, signalSemaphore, fence);
}
