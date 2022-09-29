#include "pch.h"
#include "Render/Public/ScriptableRenderer.h"
#include "Render/Passes/Public/DrawOpaquePass.h"
#include "Render/Passes/Public/MainLightShadowPass.h"
#include "Render/Passes/Public/FinalBlitPass.h"
#include "RHI/Public/RHISemaphore.h"


ScriptableRenderer::ScriptableRenderer(RendererConfigure *pConfigure)
	: m_pDevice(pConfigure->pDevice), m_pContext(pConfigure->pContext)
{
}

ScriptableRenderer::~ScriptableRenderer()
{
}

void ScriptableRenderer::Setup()
{
	RenderPassConfigure configure = {};
	configure.pDevice = m_pDevice;
	configure.pContext = m_pContext;

	m_mainLightShadowPass = (MainLightShadowPass*)WEngine::Allocator::Get()->Allocate(sizeof(MainLightShadowPass));
	::new (m_mainLightShadowPass) MainLightShadowPass(&configure);
	m_mainLightShadowPass->Setup();

	m_drawOpaquePass = (DrawOpaquePass*)WEngine::Allocator::Get()->Allocate(sizeof(DrawOpaquePass));
	::new (m_drawOpaquePass) DrawOpaquePass(&configure);
	m_drawOpaquePass->Setup();

	m_finalBlitPass = (FinalBlitPass*)WEngine::Allocator::Get()->Allocate(sizeof(FinalBlitPass));
	::new (m_finalBlitPass) FinalBlitPass(&configure);
	m_finalBlitPass->Setup();
}

void ScriptableRenderer::Execute(RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore, RHIFence *fence)
{
	m_mainLightShadowPass->Execute(waitSemaphore, signalSemaphore);

	m_drawOpaquePass->Execute(waitSemaphore, signalSemaphore, fence);

	m_finalBlitPass->Execute(waitSemaphore, signalSemaphore);
}

void ScriptableRenderer::AddRenderPass()
{

}
