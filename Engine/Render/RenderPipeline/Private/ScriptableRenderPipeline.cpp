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
#include "Scene/Components/Public/CameraComponent.h"
#include "Utils/ImGui/Public/Gui.h"

ScriptableRenderPipeline::ScriptableRenderPipeline(RHIContext *pContext)
	: m_pContext(pContext)
{
}

ScriptableRenderPipeline::~ScriptableRenderPipeline()
{
}

void ScriptableRenderPipeline::Init()
{
	m_pCameras = GWorld::GetWorld()->GetCameras();
}

void ScriptableRenderPipeline::Setup()
{
	m_pCameras = GWorld::GetWorld()->GetCameras();
	m_pCameraDatas.Reserve(m_pCameras.Size());

	for (CameraComponent *camera : m_pCameras)
	{
		CameraInfo *data = camera->GetCameraInfo();
		ScriptableRenderer* renderer = camera->GetRenderer();
		renderer->Setup(m_pContext, data);
		m_pCameraDatas.Push(data);
	}
}

void ScriptableRenderPipeline::Execute()
{
	for (CameraComponent* camera : m_pCameras)
	{
		camera->GetRenderer()->Execute(m_pContext, camera->GetCameraInfo());
	}
}

ScriptableRenderer* ScriptableRenderPipeline::CreateRenderer()
{
	ScriptableRenderer *renderer = new ScriptableRenderer(RHIContext::GetContext());
	
	return renderer;
}