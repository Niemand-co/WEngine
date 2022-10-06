#include "pch.h"
#include "Render/Public/ScriptableRenderer.h"
#include "Render/Passes/Public/DrawOpaquePass.h"
#include "Render/Passes/Public/MainLightShadowPass.h"
#include "Render/Passes/Public/FinalBlitPass.h"
#include "RHI/Public/RHISemaphore.h"
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
}

void ScriptableRenderer::Execute(RHIContext *context, RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore, RHIFence *fence)
{
	m_mainLightShadowPass->Execute(context, waitSemaphore, signalSemaphore);

	m_drawOpaquePass->Execute(context, waitSemaphore, signalSemaphore, fence);

	m_finalBlitPass->Execute(context, waitSemaphore, signalSemaphore);
}

void ScriptableRenderer::AddRenderPass()
{

}
