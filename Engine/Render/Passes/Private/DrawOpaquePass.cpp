#include "pch.h"
#include "Render/Passes/Public/DrawOpaquePass.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIRenderPass.h"
#include "RHI/Public/RHIPipelineStateObject.h"
#include "RHI/Public/RHIShader.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "RHI/Public/RHITexture.h"
#include "RHI/Public/RHITextureView.h"
#include "RHI/Public/RHISemaphore.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "RHI/Encoder/Public/RHIComputeEncoder.h"
#include "Render/Descriptor/Public/RHIRenderPassBeginDescriptor.h"
#include "Render/Descriptor/Public/RHIRenderTargetDescriptor.h"
#include "Render/Descriptor/Public/RHITextureDescriptor.h"
#include "Render/Descriptor/Public/RHITextureViewDescriptor.h"
#include "Render/Descriptor/Public/RHIRenderPassDescriptor.h"
#include "Render/Descriptor/Public/RHIPipelineStateObjectDescriptor.h"
#include "Render/Descriptor/Public/RHIShaderDescriptor.h"
#include "Render/Descriptor/Public/RHIBlendDescriptor.h"
#include "Render/Descriptor/Public/RHIDepthStencilDescriptor.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Utils/Public/Window.h"

DrawOpaquePass::DrawOpaquePass(RenderPassConfigure* configure)
	: ScriptableRenderPass(configure)
{
}

DrawOpaquePass::~DrawOpaquePass()
{
}

void DrawOpaquePass::Setup()
{
	ShaderCodeBlob* vertBlob = new ShaderCodeBlob("../assets/vert.spv");
	RHIShaderDescriptor vertShaderDescriptor = {};
	{
		vertShaderDescriptor.entryName = "vert";
		vertShaderDescriptor.shaderStage = ShaderStage::vertex;
		vertShaderDescriptor.pCode = vertBlob->GetCode();
		vertShaderDescriptor.codeSize = vertBlob->GetSize();
	}
	RHIShader* vertShader = m_pDevice->CreateShader(&vertShaderDescriptor);

	ShaderCodeBlob* fragBlob = new ShaderCodeBlob("../assets/frag.spv");
	RHIShaderDescriptor fragShaderDescriptor = {};
	{
		fragShaderDescriptor.entryName = "frag";
		fragShaderDescriptor.shaderStage = ShaderStage::fragment;
		fragShaderDescriptor.pCode = fragBlob->GetCode();
		fragShaderDescriptor.codeSize = fragBlob->GetSize();
	}
	RHIShader* fragShader = m_pDevice->CreateShader(&fragShaderDescriptor);

	RHIRenderPassDescriptor renderPassDescriptor = {};
	{
		renderPassDescriptor.attachmentCount = 1;
		renderPassDescriptor.attachmentFormat = Format::A16R16G16B16_SFloat;
		renderPassDescriptor.attachmentLoadOP = AttachmentLoadOP::Clear;
		renderPassDescriptor.attachmentStoreOP = AttachmentStoreOP::Store;
		renderPassDescriptor.stencilLoadOP = AttachmentLoadOP::Clear;
		renderPassDescriptor.stencilStoreOP = AttachmentStoreOP::Store;
		renderPassDescriptor.sampleCount = 1;
	}
	m_pRenderPass = m_pDevice->CreateRenderPass(&renderPassDescriptor);

	std::vector<RHIShader*> shaders = { vertShader, fragShader };
	RHIBlendDescriptor blendDescriptor = {};
	{
		blendDescriptor.blendEnabled = false;
		blendDescriptor.colorSrcFactor = BlendFactor::FactorOne;
		blendDescriptor.colorDstFactor = BlendFactor::FactorZero;
		blendDescriptor.colorBlendOP = BlendOP::BlendAdd;
		blendDescriptor.alphaSrcFactor = BlendFactor::FactorOne;
		blendDescriptor.alphaDstFactor = BlendFactor::FactorZero;
		blendDescriptor.alphaBlendOP = BlendOP::BlendAdd;
	}
	RHIDepthStencilDescriptor depthStencilDescriptor = {};
	{
		depthStencilDescriptor.depthWriteEnabled = true;
		depthStencilDescriptor.depthTestEnabled = true;
		depthStencilDescriptor.depthCompareOP = DepthCompareOP::Greater;
		depthStencilDescriptor.depthBoundsTest = false;
		depthStencilDescriptor.maxDepth = 1.0f;
		depthStencilDescriptor.minDepth = 0.0f;
	}
	RHIPipelineStateObjectDescriptor psoDescriptor = {};
	{
		psoDescriptor.renderPass = m_pRenderPass;
		psoDescriptor.blendDescriptor = &blendDescriptor;
		psoDescriptor.depthStencilDescriptor = &depthStencilDescriptor;
		psoDescriptor.pShader = shaders.data();
		psoDescriptor.shaderCount = shaders.size();
	}
	m_pPSO = m_pDevice->CreatePipelineStateObject(&psoDescriptor);
}

void DrawOpaquePass::Execute(RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore)
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
