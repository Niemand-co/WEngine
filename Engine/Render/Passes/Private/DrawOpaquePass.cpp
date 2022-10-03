#include "pch.h"
#include "Render/Passes/Public/DrawOpaquePass.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIRenderPass.h"
#include "RHI/Public/RHIPipelineStateObject.h"
#include "RHI/Public/RHIShader.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "RHI/Public/RHITextureView.h"
#include "RHI/Public/RHIRenderTarget.h"
#include "RHI/Public/RHISemaphore.h"
#include "RHI/Public/RHIBuffer.h"
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
#include "Render/Descriptor/Public/RHIVertexInputDescriptor.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"
#include "Utils/Public/Window.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Render/Mesh/Public/Vertex.h"

DrawOpaquePass::DrawOpaquePass(RenderPassConfigure* configure)
	: ScriptableRenderPass(configure)
{
}

DrawOpaquePass::~DrawOpaquePass()
{
}

void DrawOpaquePass::Setup(RHIContext *context)
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

	m_pMesh = (Mesh*)WEngine::Allocator::Get()->Allocate(sizeof(Mesh));
	::new (m_pMesh) Mesh();
	m_pMesh->m_pVertices = (Vertex*)WEngine::Allocator::Get()->Allocate(3 * sizeof(Vertex));
	m_pMesh->m_pVertices[0].Position = { -0.5f, -0.5f, 0.0f };
	m_pMesh->m_pVertices[1].Position = { 0.5f, -0.5f, 0.0f };
	m_pMesh->m_pVertices[2].Position = { 0.0f, 0.5f, 0.0f };
	m_pMesh->m_pVertices[0].Color = { 1.0f, 0.0f, 0.0f };
	m_pMesh->m_pVertices[1].Color = { 0.0f, 1.0f, 0.0f };
	m_pMesh->m_pVertices[2].Color = { 0.0f, 0.0f, 1.0f };
	m_pMesh->m_pVertices[0].UV = { 0.0f, 0.0f };
	m_pMesh->m_pVertices[1].UV = { 0.0f, 0.0f };
	m_pMesh->m_pVertices[2].UV = { 0.0f, 0.0f };
	m_pMesh->m_vertexCount = 3;

	m_pMesh->GenerateVertexInputDescription();
	RHIVertexInputDescriptor vertexInputDescriptor = m_pMesh->GetVertexInputDescriptor();
	RHIPipelineStateObjectDescriptor psoDescriptor = {};
	{
		psoDescriptor.renderPass = m_pRenderPass;
		psoDescriptor.blendDescriptor = &blendDescriptor;
		psoDescriptor.depthStencilDescriptor = &depthStencilDescriptor;
		psoDescriptor.pShader = shaders.data();
		psoDescriptor.shaderCount = shaders.size();
		psoDescriptor.vertexDescriptor = &vertexInputDescriptor;
	};
	m_pPSO = m_pDevice->CreatePipelineStateObject(&psoDescriptor);


	RHIBufferDescriptor bufferDescriptor = {};
	{
		bufferDescriptor.size = m_pMesh->m_vertexCount * sizeof(Vertex);
		bufferDescriptor.pData = m_pMesh->m_pVertices;
	}
	m_pBuffer = m_pDevice->CreateBuffer(&bufferDescriptor);

	m_pRenderTargets.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		std::vector<RHITextureView*> textureViews = { context->GetTextureView(i) };
		RHIRenderTargetDescriptor renderTargetDescriptor = {};
		{
			renderTargetDescriptor.bufferCount = 1;
			renderTargetDescriptor.pBufferView = textureViews.data();
			renderTargetDescriptor.renderPass = m_pRenderPass;
			renderTargetDescriptor.width = Window::cur_window->GetWidth();
			renderTargetDescriptor.height = Window::cur_window->GetHeight();
		}
		m_pRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
	}
}

void DrawOpaquePass::Execute(RHIContext *context, RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence *fence)
{
	if (context->IsDisplayChanged())
	{
		for (int i = 0; i < 3; ++i)
		{
			std::vector<RHITextureView*> textureViews = { context->GetTextureView(i) };
			RHIRenderTargetDescriptor renderTargetDescriptor = {};
			{
				renderTargetDescriptor.bufferCount = 1;
				renderTargetDescriptor.pBufferView = textureViews.data();
				renderTargetDescriptor.renderPass = m_pRenderPass;
				renderTargetDescriptor.width = Window::cur_window->GetWidth();
				renderTargetDescriptor.height = Window::cur_window->GetHeight();
			}
			m_pRenderTargets[i]->~RHIRenderTarget();
			WEngine::Allocator::Get()->Deallocate(m_pRenderTargets[i]);
			m_pRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
		}
		context->ResetDisplayState();
	}

	RHICommandBuffer* cmd = context->GetCommandBuffer();

	cmd->BeginScopePass("Test");
	{
		RHIGraphicsEncoder* encoder = cmd->GetGraphicsEncoder();
		RHIRenderPassBeginDescriptor renderPassBeginDescriptor = {};
		{
			renderPassBeginDescriptor.renderPass = m_pRenderPass;
			renderPassBeginDescriptor.renderTarget = m_pRenderTargets[ScriptableRenderPipeline::g_currentFrame];
		}
		encoder->BeginPass(&renderPassBeginDescriptor);
		encoder->SetPipeline(m_pPSO);
		encoder->SetViewport(nullptr);
		encoder->SetScissor(nullptr);
		encoder->BindVertexBuffer(m_pBuffer);
		encoder->DrawVertexArray();
		encoder->EndPass();
		cmd->EndScopePass();
		context->ExecuteCommandBuffer(cmd);
		context->Submit(waitSemaphore, signalSemaphore, fence);
		encoder->~RHIGraphicsEncoder();
		WEngine::Allocator::Get()->Deallocate(encoder);
	}
	cmd->Clear();

	cmd->~RHICommandBuffer();
	WEngine::Allocator::Get()->Deallocate(cmd);
}
