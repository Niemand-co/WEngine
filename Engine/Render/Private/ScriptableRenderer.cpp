#include "pch.h"
#include "Render/Public/ScriptableRenderer.h"
#include "Render/Passes/Public/ScriptableRenderPass.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIRenderPass.h"
#include "RHI/Public/RHIPipelineStateObject.h"
#include "RHI/Public/RHIShader.h"
#include "RHI/Public/RHISemaphore.h"
#include "Render/Descriptor/Public/RHIRenderPassDescriptor.h"
#include "Render/Descriptor/Public/RHIPipelineStateObjectDescriptor.h"
#include "Render/Descriptor/Public/RHIShaderDescriptor.h"
#include "Render/Descriptor/Public/RHIBlendDescriptor.h"
#include "Render/Descriptor/Public/RHIDepthStencilDescriptor.h"

ScriptableRenderer::ScriptableRenderer(RendererConfigure *pConfigure)
	: m_pDevice(pConfigure->pDevice), m_pContext(pConfigure->pContext)
{
}

ScriptableRenderer::~ScriptableRenderer()
{
}

void ScriptableRenderer::Setup()
{
}

void ScriptableRenderer::Execute(RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore)
{
	for (ScriptableRenderPass* pass : m_passes)
	{
		pass->Execute(waitSemaphore, signalSemaphore);
	}
}

void ScriptableRenderer::AddRenderPass()
{
	RenderPassConfigure configure = {};

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
	RHIRenderPass* renderPass = m_pDevice->CreateRenderPass(&renderPassDescriptor);

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
		psoDescriptor.renderPass = renderPass;
		psoDescriptor.blendDescriptor = &blendDescriptor;
		psoDescriptor.depthStencilDescriptor = &depthStencilDescriptor;
		psoDescriptor.pShader = shaders.data();
		psoDescriptor.shaderCount = shaders.size();
	}
	RHIPipelineStateObject* pso = m_pDevice->CreatePipelineStateObject(&psoDescriptor);

	configure.pDevice = m_pDevice;
	configure.pContext = m_pContext;
	configure.pRenderPass = renderPass;
	configure.pPSO = pso;

	ScriptableRenderPass *pass = (ScriptableRenderPass*)Allocator::Allocate(sizeof(ScriptableRenderPass));
	::new (pass) ScriptableRenderPass(&configure);
	m_passes.push_back(pass);
}
