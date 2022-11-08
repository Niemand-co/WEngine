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

ScriptableRenderer::ScriptableRenderer(RHIContext* pContext)
	: m_pContext(pContext)
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

	m_pSignalSemaphore = RHIContext::GetDevice()->GetSemaphore(1);
	m_blockSubmission.push_back("Gui");

	WEngine::Trigger *trigger = new WEngine::Trigger();
	trigger->signal = m_pSignalSemaphore[0];
	trigger->waitingSubmissionCount = m_blockSubmission.size();
	trigger->pSubmissionNames = m_blockSubmission.data();
	WEngine::Synchronizer::RegisterTrigger(trigger);
}

void ScriptableRenderer::Execute(RHIContext *context, CameraData *cameraData)
{
	for (unsigned int i = 0; i < m_passes.size(); ++i)
	{
		m_passes[i]->Execute(context, cameraData);
	}

	RHISemaphore *waitSemaphores[] = { context->GetImageVailableSemaphore() };

	RHISubmitDescriptor submitDescriptor = {};
	{
		submitDescriptor.waitSemaphoreCount = 1;
		submitDescriptor.pWaitSemaphores = waitSemaphores;
		submitDescriptor.signalSemaphoreCount = m_pSignalSemaphore.size();
		submitDescriptor.pSignalSemaphores = m_pSignalSemaphore.data();
		submitDescriptor.waitStage = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		submitDescriptor.pFence = nullptr;
	}
	context->Submit(&submitDescriptor);
}

void ScriptableRenderer::EnqueRenderPass(ScriptableRenderPass *renderPass)
{
	m_passes.push_back(renderPass);
}
