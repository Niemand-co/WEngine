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

ScriptableRenderPipeline::ScriptableRenderPipeline(RHIContext *pContext)
	: m_pContext(pContext)
{
}

ScriptableRenderPipeline::~ScriptableRenderPipeline()
{
}

void ScriptableRenderPipeline::Init()
{
	m_pCameras = World::GetWorld()->GetCameras();
}

void ScriptableRenderPipeline::Setup()
{
	m_pCameras = World::GetWorld()->GetCameras();
	m_pCameraDatas.Reserve(m_pCameras.Size());

	for (Camera *camera : m_pCameras)
	{
		CameraData *data = camera->GetData();
		ScriptableRenderer* renderer = camera->GetRenderer();
		renderer->EnqueRenderPass(new MainLightShadowPass(renderer));
		renderer->EnqueRenderPass(new DrawOpaquePass(renderer));
		renderer->EnqueRenderPass(new DrawSkyboxPass(renderer));
		renderer->EnqueRenderPass(new DrawGizmosPass(renderer));
		renderer->Setup(m_pContext, data);
		m_pCameraDatas.Push(data);
	}
}

void ScriptableRenderPipeline::Execute()
{
	for (Camera* camera : m_pCameras)
	{
		camera->GetRenderer()->Execute(m_pContext, camera->GetData());
	}
}

ScriptableRenderer* ScriptableRenderPipeline::CreateRenderer()
{
	ScriptableRenderer *renderer = new ScriptableRenderer(RHIContext::GetContext());
	
	return renderer;
}