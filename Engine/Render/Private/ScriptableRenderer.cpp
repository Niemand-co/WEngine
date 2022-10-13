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
	: m_pDevice(pConfigure->pDevice), m_pContext(pConfigure->pContext)
{
}

ScriptableRenderer::~ScriptableRenderer()
{
}

void ScriptableRenderer::Setup(CameraData* cameraData)
{
	RenderPassConfigure configure = {};
	configure.pDevice = m_pDevice;

	m_mainLightShadowPass = (MainLightShadowPass*)WEngine::Allocator::Get()->Allocate(sizeof(MainLightShadowPass));
	::new (m_mainLightShadowPass) MainLightShadowPass(&configure);
	m_mainLightShadowPass->Setup(m_pContext, cameraData);

	m_drawOpaquePass = (DrawOpaquePass*)WEngine::Allocator::Get()->Allocate(sizeof(DrawOpaquePass));
	::new (m_drawOpaquePass) DrawOpaquePass(&configure);
	m_drawOpaquePass->Setup(m_pContext, cameraData);

	m_finalBlitPass = (FinalBlitPass*)WEngine::Allocator::Get()->Allocate(sizeof(FinalBlitPass));
	::new (m_finalBlitPass) FinalBlitPass(&configure);
	m_finalBlitPass->Setup(m_pContext, cameraData);

	m_drawGuiPass = (DrawGUIPass*)WEngine::Allocator::Get()->Allocate(sizeof(DrawGUIPass));
	::new (m_drawGuiPass) DrawGUIPass(&configure);
	m_drawGuiPass->Setup(m_pContext, cameraData);

	m_semaphores = m_pDevice->GetSemaphore(3);

	m_pEvent = m_pDevice->GetEvent();
}

void ScriptableRenderer::Execute(RHIContext *context, RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore, RHIFence *fence)
{
	m_mainLightShadowPass->Execute(context, waitSemaphore, signalSemaphore);

	m_drawOpaquePass->Execute(context, waitSemaphore, m_semaphores[ScriptableRenderPipeline::g_currentFrame], fence, m_pEvent);

	m_drawGuiPass->Execute(context, m_semaphores[ScriptableRenderPipeline::g_currentFrame], signalSemaphore, nullptr, m_pEvent);

	m_finalBlitPass->Execute(context, waitSemaphore, signalSemaphore);
}

void ScriptableRenderer::AddRenderPass()
{

}
