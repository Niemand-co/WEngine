#include "pch.h"
#include "Render/Public/ScriptableRenderer.h"
#include "Render/Passes/Public/DrawOpaquePass.h"
#include "Render/Passes/Public/MainLightShadowPass.h"
#include "Render/Passes/Public/FinalBlitPass.h"
#include "Render/Passes/Public/DrawGUIPass.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "RHI/Public/RHIHeads.h"
#include "Scene/Components/Public/Camera.h"

ScriptableRenderer::ScriptableRenderer(RendererConfigure *pConfigure)
	: m_pDevice(pConfigure->pDevice)
{
}

ScriptableRenderer::~ScriptableRenderer()
{
}

void ScriptableRenderer::Setup(RHIContext* context, CameraData* cameraData)
{
	for (ScriptableRenderPass* pass : m_passes)
	{
		pass->Setup(context, cameraData);
	}

	m_semaphores = m_pDevice->GetSemaphore(ScriptableRenderPipeline::g_maxFrame * (m_passes.size() - 1));
}

void ScriptableRenderer::Execute(RHIContext *context, RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore, RHIFence *fence)
{
	for (unsigned int i = 0; i < m_passes.size(); ++i)
	{
		m_passes[i]->Execute(context);
	}

	RHISubmitDescriptor submitDescriptor = {};
	{
		submitDescriptor.waitSemaphoreCount = 1;
		submitDescriptor.pWaitSemaphores = &waitSemaphore;
		submitDescriptor.signalSemaphoreCount = 1;
		submitDescriptor.pSignalSemaphores = &signalSemaphore;
		submitDescriptor.waitStage = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		submitDescriptor.pFence = fence;
	}
	context->Submit(&submitDescriptor);
}

void ScriptableRenderer::EnqueRenderPass(ScriptableRenderPass *renderPass)
{
	m_passes.push_back(renderPass);
}
