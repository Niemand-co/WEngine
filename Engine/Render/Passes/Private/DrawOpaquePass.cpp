#include "pch.h"
#include "Render/Passes/Public/DrawOpaquePass.h"
#include "RHI/Public/RHIHeads.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "RHI/Encoder/Public/RHIComputeEncoder.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Utils/Public/Window.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Scene/Components/Public/Camera.h"

DrawOpaquePass::DrawOpaquePass(RenderPassConfigure* configure)
	: ScriptableRenderPass(configure)
{
}

DrawOpaquePass::~DrawOpaquePass()
{
}

void DrawOpaquePass::Setup(RHIContext *context, CameraData *cameraData)
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

	m_pMesh = Mesh::GetCube();

	BindingResource resource[1] = 
	{
		{0, ResourceType::UniformBuffer, 1, ShaderStage::vertex}
	};
	RHIGroupLayoutDescriptor groupLayoutDescriptor = {};
	{
		groupLayoutDescriptor.bindingCount = 1;
		groupLayoutDescriptor.pBindingResources = resource;
	}
	RHIGroupLayout *groupLayout = context->CreateGroupLayout(&groupLayoutDescriptor);

	RHIPipelineResourceLayoutDescriptor pipelineResourceLayoutDescriptor = {};
	{
		pipelineResourceLayoutDescriptor.groupLayoutCount = 1;
		pipelineResourceLayoutDescriptor.pGroupLayout = groupLayout;
	}
	m_pPipelineResourceLayout = context->CreatePipelineResourceLayout(&pipelineResourceLayoutDescriptor);

	RHIGroupDescriptor groupDescriptor = {};
	{
		groupDescriptor.pGroupLayout = groupLayout;
	}
	m_pGroup = context->CreateResourceGroup(&groupDescriptor);

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
		psoDescriptor.pipelineResourceLayout = m_pPipelineResourceLayout;
	};
	m_pPSO = context->CreatePSO(&psoDescriptor);


	RHIBufferDescriptor bufferDescriptor = {};
	{
		bufferDescriptor.size = m_pMesh->m_vertexCount * sizeof(Vertex);
		bufferDescriptor.pData = m_pMesh->m_pVertices;
		bufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pVertexBuffer = context->CreateVertexBuffer(&bufferDescriptor);

	RHIBufferDescriptor indexBufferDescriptor = {};
	{
		indexBufferDescriptor.size = m_pMesh->m_indexCount * sizeof(unsigned int);
		indexBufferDescriptor.pData = m_pMesh->m_pIndices;
		indexBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pIndexBuffer = context->CreateIndexBuffer(&indexBufferDescriptor);

	glm::mat4x4 uniformData[] = { cameraData->MatrixVP };
	RHIBufferDescriptor uniformBufferDescriptor = {};
	{
		uniformBufferDescriptor.size = sizeof(uniformData);
		uniformBufferDescriptor.pData = uniformData;
		uniformBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pUniformBuffer = context->CreateUniformBuffer(&uniformBufferDescriptor);

	size_t pSizes[1] = {sizeof(uniformData)};
	RHIUpdateResourceDescriptor updateResourceDescriptor = {};
	{
		updateResourceDescriptor.bindingCount = 1;
		updateResourceDescriptor.pBindingResources = resource;
		updateResourceDescriptor.pBuffer = m_pUniformBuffer;
		updateResourceDescriptor.pSize = pSizes;
		updateResourceDescriptor.pGroup = m_pGroup;
	}
	context->UpdateResourceToGroup(&updateResourceDescriptor);

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
	cmd->Clear();

	cmd->BeginScopePass("Test");
	{
		RHIGraphicsEncoder* encoder = cmd->GetGraphicsEncoder();
		RHIRenderPassBeginDescriptor renderPassBeginDescriptor = {};
		{
			renderPassBeginDescriptor.renderPass = m_pRenderPass;
			renderPassBeginDescriptor.renderTarget = m_pRenderTargets[ScriptableRenderPipeline::g_currentImage];
		}
		encoder->BeginPass(&renderPassBeginDescriptor);
		encoder->SetPipeline(m_pPSO);
		encoder->SetViewport(nullptr);
		encoder->SetScissor(nullptr);
		encoder->BindVertexBuffer(m_pVertexBuffer);
		encoder->BindIndexBuffer(m_pIndexBuffer);
		encoder->BindGroups(1, m_pGroup, m_pPipelineResourceLayout);
		encoder->DrawIndexed(m_pMesh->m_indexCount, 0);
		encoder->EndPass();
		encoder->~RHIGraphicsEncoder();
		WEngine::Allocator::Get()->Deallocate(encoder);
	}
	cmd->EndScopePass();
	context->ExecuteCommandBuffer(cmd);
	RHISubmitDescriptor submitDescriptor = {};
	{
		submitDescriptor.waitSemaphoreCount = 1;
		submitDescriptor.pWaitSemaphores = &waitSemaphore;
		submitDescriptor.signalSemaphoreCount = 1;
		submitDescriptor.pSignalSemaphores = &signalSemaphore;
		submitDescriptor.waitStage = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		submitDescriptor.pFence = fence;
	}
	context->Submit(&submitDescriptor);
	cmd->Clear();

	cmd->~RHICommandBuffer();
	WEngine::Allocator::Get()->Deallocate(cmd);
}
