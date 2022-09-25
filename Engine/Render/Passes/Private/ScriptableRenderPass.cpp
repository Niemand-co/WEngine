#include "pch.h"
#include "Render/Passes/Public/ScriptableRenderPass.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "RHI/Public/RHIRenderPass.h"
#include "RHI/Public/RHIPipelineStateObject.h"
#include "RHI/Public/RHITexture.h"
#include "RHI/Public/RHITextureView.h"
#include "RHI/Public/RHISemaphore.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "RHI/Encoder/Public/RHIComputeEncoder.h"
#include "Render/Descriptor/Public/RHIRenderPassBeginDescriptor.h"
#include "Render/Descriptor/Public/RHIRenderTargetDescriptor.h"
#include "Render/Descriptor/Public/RHITextureDescriptor.h"
#include "Render/Descriptor/Public/RHITextureViewDescriptor.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Utils/Public/Window.h"

ScriptableRenderPass::ScriptableRenderPass(RenderPassConfigure* configure)
	: m_pContext(configure->pContext), m_pRenderPass(configure->pRenderPass), m_pPSO(configure->pPSO), m_pDevice(configure->pDevice)
{
}

ScriptableRenderPass::~ScriptableRenderPass()
{
}

void ScriptableRenderPass::Setup()
{
}

void ScriptableRenderPass::Execute(RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore)
{
	RHICommandBuffer* cmd = m_pContext->GetCommandBuffer();

	//RHITextureDescriptor textureDescriptor = {};
	//{
	//	textureDescriptor.format = Format::A16R16G16B16_SFloat;
	//	textureDescriptor.width = Window::cur_window->GetWidth();
	//	textureDescriptor.height = Window::cur_window->GetHeight();
	//	textureDescriptor.mipCount = 0;
	//}
	//RHITexture* renderTexture = m_pDevice->CreateTexture(&textureDescriptor);

	RHITextureViewDescriptor textureViewDescriptor = {};
	{
		textureViewDescriptor.format = Format::A16R16G16B16_SFloat;
		textureViewDescriptor.mipCount = 1;
		textureViewDescriptor.baseMipLevel = 0;
		textureViewDescriptor.arrayLayerCount = 1;
		textureViewDescriptor.baseArrayLayer = 0;
		textureViewDescriptor.dimension = Dimension::Texture2D;
	}
	RHITextureView* view = ScriptableRenderPipeline::g_currentRenderTarget->CreateTextureView(&textureViewDescriptor);

	RHIRenderTargetDescriptor renderTargetDescriptor = {};
	{
		renderTargetDescriptor.bufferCount = 1;
		renderTargetDescriptor.pBufferView = &view;
		renderTargetDescriptor.renderPass = m_pRenderPass;
		renderTargetDescriptor.width = Window::cur_window->GetWidth();
		renderTargetDescriptor.height = Window::cur_window->GetHeight();
	}
	RHIRenderTarget* renderTarget = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);

	cmd->BeginScopePass("Test");
	RHIGraphicsEncoder* encoder = cmd->GetGraphicsEncoder();
	RHIRenderPassBeginDescriptor renderPassBeginDescriptor = {};
	{
		renderPassBeginDescriptor.renderPass = m_pRenderPass;
		renderPassBeginDescriptor.renderTarget = renderTarget;
	}
	encoder->BeginPass(&renderPassBeginDescriptor);
	encoder->SetPipeline(m_pPSO);
	encoder->SetViewport(nullptr);
	encoder->SetScissor(nullptr);
	encoder->EndPass();
	cmd->EndScopePass();
	m_pContext->ExecuteCommandBuffer(cmd);
	m_pContext->Submit(waitSemaphore, signalSemaphore);

}
