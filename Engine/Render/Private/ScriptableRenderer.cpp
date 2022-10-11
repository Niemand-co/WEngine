#include "pch.h"
#include "Render/Public/ScriptableRenderer.h"
#include "Render/Passes/Public/DrawOpaquePass.h"
#include "Render/Passes/Public/MainLightShadowPass.h"
#include "Render/Passes/Public/FinalBlitPass.h"
#include "Render/Passes/Public/DrawGUIPass.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "RHI/Public/RHIDevice.h"
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

	RHIAttachmentDescriptor attachmentDescriptors[] = 
	{
		{ Format::A16R16G16B16_SFloat, 1, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLoadOP::Clear, AttachmentStoreOP::DontCare, AttachmentLayout::Undefined, AttachmentLayout::ColorBuffer },
		{ Format::A16R16G16B16_SFloat, 1, AttachmentLoadOP::Load, AttachmentStoreOP::Store, AttachmentLoadOP::Load, AttachmentStoreOP::Store, AttachmentLayout::Undefined, AttachmentLayout::ColorBuffer },
		//{ Format::D32_SFloat, 1, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLayout::Undefined, AttachmentLayout::DepthBuffer }
	};
	RHISubPassDescriptor subpassDescriptors[] = 
	{
		{ 0, AttachmentLayout::ColorBuffer, -1, PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT, 0, PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT, ACCESS_COLOR_ATTACHMENT_WRITE | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE },
		{ 1, AttachmentLayout::ColorBuffer, 0, PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT, 0, PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT, ACCESS_COLOR_ATTACHMENT_WRITE | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE }
	};
	RHIRenderPassDescriptor renderPassDescriptor = {};
	{
		renderPassDescriptor.attachmentCount = 2;
		renderPassDescriptor.pAttachmentDescriptors = attachmentDescriptors;
		renderPassDescriptor.subpassCount = 2;
		renderPassDescriptor.pSubPassDescriptors = subpassDescriptors;
	}
	configure.pRenderPass = m_pDevice->CreateRenderPass(&renderPassDescriptor);

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
}

void ScriptableRenderer::Execute(RHIContext *context, RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore, RHIFence *fence)
{
	m_mainLightShadowPass->Execute(context, waitSemaphore, signalSemaphore);

	m_drawOpaquePass->Execute(context, waitSemaphore, m_semaphores[ScriptableRenderPipeline::g_currentFrame], fence);

	m_drawGuiPass->Execute(context, m_semaphores[ScriptableRenderPipeline::g_currentFrame], signalSemaphore);

	m_finalBlitPass->Execute(context, waitSemaphore, signalSemaphore);
}

void ScriptableRenderer::AddRenderPass()
{

}
