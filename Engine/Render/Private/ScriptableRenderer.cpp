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

	m_mainLightShadowPass = (MainLightShadowPass*)Allocator::Allocate(sizeof(MainLightShadowPass));
	::new (m_mainLightShadowPass) MainLightShadowPass(&configure);

	m_drawOpaquePass = (DrawOpaquePass*)Allocator::Allocate(sizeof(DrawOpaquePass));
	::new (m_drawOpaquePass) DrawOpaquePass(&configure);

	m_finalBlitPass = (FinalBlitPass*)Allocator::Allocate(sizeof(FinalBlitPass));
	::new (m_finalBlitPass) FinalBlitPass(&configure);
}

void ScriptableRenderer::Execute(RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore)
{
	m_mainLightShadowPass->Execute(waitSemaphore, signalSemaphore);

	m_drawOpaquePass->Execute(waitSemaphore, signalSemaphore);

	m_finalBlitPass->Execute(waitSemaphore, signalSemaphore);
}

void ScriptableRenderer::AddRenderPass()
{

}
