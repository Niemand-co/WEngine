#include "pch.h"
#include "Render/Passes/Public/DrawOpaquePass.h"
#include "RHI/Public/RHIHeads.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "RHI/Encoder/Public/RHIComputeEncoder.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Utils/Public/Window.h"
#include "Utils/ImGui/Public/Gui.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Scene/Public/GameObject.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"

struct UniformData
{
	glm::mat4 VP;
	glm::vec4 lightPos;
	glm::vec4 cameraPos;
	glm::vec4 surfaceData;
};

DrawOpaquePass::DrawOpaquePass()
{
}

DrawOpaquePass::~DrawOpaquePass()
{
}

void DrawOpaquePass::Setup(RHIContext *context, CameraData *cameraData)
{
	RHIAttachmentDescriptor attachmentDescriptors[] =
	{
		{ Format::A16R16G16B16_SFloat, 1, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLoadOP::Clear, AttachmentStoreOP::DontCare, AttachmentLayout::Undefined, AttachmentLayout::Present },
		{ Format::D16_Unorm, 1, AttachmentLoadOP::Clear, AttachmentStoreOP::DontCare, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLayout::Undefined, AttachmentLayout::DepthBuffer }
	};
	SubPassAttachment subpassColorAttachment = { 0, AttachmentLayout::ColorBuffer };
	SubPassAttachment subpassDepthAttachment = { 1, AttachmentLayout::DepthBuffer };

	RHISubPassDescriptor subpassDescriptors = {};
	{
		subpassDescriptors.colorAttachmentCount = 1;
		subpassDescriptors.pColorAttachments = &subpassColorAttachment;
		subpassDescriptors.pDepthStencilAttachment = &subpassDepthAttachment;
		subpassDescriptors.dependedStage = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		subpassDescriptors.waitingStage = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		subpassDescriptors.waitingAccess = ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE;
	}
	RHIRenderPassDescriptor renderPassDescriptor = {};
	{
		renderPassDescriptor.attachmentCount = 2;
		renderPassDescriptor.pAttachmentDescriptors = attachmentDescriptors;
		renderPassDescriptor.subpassCount = 1;
		renderPassDescriptor.pSubPassDescriptors = &subpassDescriptors;
	}
	m_pRenderPass = m_pDevice->CreateRenderPass(&renderPassDescriptor);

	ShaderCodeBlob* vertBlob = new ShaderCodeBlob("../assets/vert.spv");
	RHIShaderDescriptor vertShaderDescriptor = {};
	{
		vertShaderDescriptor.entryName = "vert";
		vertShaderDescriptor.shaderStage = SHADER_STAGE_VERTEX;
		vertShaderDescriptor.pCode = vertBlob->GetCode();
		vertShaderDescriptor.codeSize = vertBlob->GetSize();
	}
	RHIShader* vertShader = m_pDevice->CreateShader(&vertShaderDescriptor);

	ShaderCodeBlob* fragBlob = new ShaderCodeBlob("../assets/frag.spv");
	RHIShaderDescriptor fragShaderDescriptor = {};
	{
		fragShaderDescriptor.entryName = "frag";
		fragShaderDescriptor.shaderStage = SHADER_STAGE_FRAGMENT;
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
		depthStencilDescriptor.depthCompareOP = DepthCompareOP::Less;
		depthStencilDescriptor.depthBoundsTest = false;
		depthStencilDescriptor.maxDepth = 1.0f;
		depthStencilDescriptor.minDepth = 0.0f;
	}

	GameObject *go = GameObject::Find("Cube");
	m_pMesh = go->GetComponent<MeshFilter>()->GetStaticMesh();

	BindingResource resource[2] = 
	{
		{0, ResourceType::UniformBuffer, 1, SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT},
		{1, ResourceType::CombinedImageSampler, 1, SHADER_STAGE_FRAGMENT},
	};
	RHIGroupLayoutDescriptor groupLayoutDescriptor = {};
	{
		groupLayoutDescriptor.bindingCount = 2;
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

	SurfaceData surfaceData = go->GetComponent<Material>()->GetSurfaceData();

	UniformData data = 
	{
		cameraData->MatrixVP,
		glm::vec4(-2.0f, 2.0f, -2.0f, 1.0f),
		glm::vec4(cameraData->Position, 1.0f),
		glm::vec4(surfaceData.albedo, surfaceData.roughness)
	};
	RHIBufferDescriptor uniformBufferDescriptor = {};
	{
		uniformBufferDescriptor.size = sizeof(data);
		uniformBufferDescriptor.pData = &data;
		uniformBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pUniformBuffer = context->CreateUniformBuffer(&uniformBufferDescriptor);

	BufferResourceInfo bufferInfo[] = 
	{
		{ m_pUniformBuffer, 0, sizeof(data) },
	};
	RHIUpdateResourceDescriptor updateResourceDescriptor = {};
	{
		updateResourceDescriptor.bindingCount = 1;
		updateResourceDescriptor.pBindingResources = resource;
		updateResourceDescriptor.pGroup = m_pGroup;
		updateResourceDescriptor.bufferResourceCount = 1;
		updateResourceDescriptor.pBufferInfo = bufferInfo;
	}
	context->UpdateUniformResourceToGroup(&updateResourceDescriptor);

	RHITextureViewDescriptor uvd = {};
	{
		uvd.format = Format::A8R8G8B8_UNorm;
		uvd.arrayLayerCount = 1;
		uvd.baseArrayLayer = 0;
		uvd.mipCount = 1;
		uvd.baseMipLevel = 0;
		uvd.dimension = Dimension::Texture2D;
		uvd.imageAspect = IMAGE_ASPECT_COLOR;
	}
	
	RHITextureDescriptor textureDescriptor = {};
	{
		textureDescriptor.format = Format::A8R8G8B8_UNorm;
		textureDescriptor.width = 477;
		textureDescriptor.height = 377;
		textureDescriptor.layout = AttachmentLayout::Undefined;
		textureDescriptor.mipCount = 1;
		textureDescriptor.usage = IMAGE_USAGE_TRANSFER_DST | IMAGE_USAGE_SAMPLED;
	}
	m_pTexture = m_pDevice->CreateTexture(&textureDescriptor);
	m_pTexture->LoadData("assets/chino.png", context);

	RHISamplerDescriptor samplerDescriptor = {};
	{
		samplerDescriptor.minFilter = Filter::Linear;
		samplerDescriptor.magFilter = Filter::Linear;
	}
	m_pSampler = m_pDevice->CreateSampler(&samplerDescriptor);

	TextureResourceInfo textureInfo[] = 
	{
		{ m_pTexture->CreateTextureView(&uvd), m_pSampler, AttachmentLayout::ReadOnlyColor },
	};
	RHIUpdateResourceDescriptor textureResourceDescriptor = {};
	{
		textureResourceDescriptor.bindingCount = 1;
		textureResourceDescriptor.pBindingResources = resource + 1;
		textureResourceDescriptor.pGroup = m_pGroup;
		textureResourceDescriptor.textureResourceCount = 1;
		textureResourceDescriptor.pTextureInfo = textureInfo;
	}
	context->UpdateTextureResourceToGroup(&textureResourceDescriptor);

	m_pRenderTargets.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		std::vector<RHITextureView*> textureViews = { context->GetTextureView(i), context->GetDepthView(i) };
		RHIRenderTargetDescriptor renderTargetDescriptor = {};
		{
			renderTargetDescriptor.bufferCount = 2;
			renderTargetDescriptor.pBufferView = textureViews.data();
			renderTargetDescriptor.renderPass = m_pRenderPass;
			renderTargetDescriptor.width = Window::cur_window->GetWidth();
			renderTargetDescriptor.height = Window::cur_window->GetHeight();
		}
		m_pRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
	}

	m_pCommandBuffers = context->GetCommandBuffer(RHIContext::g_maxFrames, false);
}

void DrawOpaquePass::Execute(RHIContext *context, CameraData *cameraData)
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

	RHICommandBuffer *cmd = m_pCommandBuffers[RHIContext::g_currentFrame];

	SurfaceData surfaceData = GameObject::Find("Cube")->GetComponent<Material>()->GetSurfaceData();
	UniformData data =
	{
		cameraData->MatrixVP,
		glm::vec4(-2.0f, 2.0f, -2.0f, 1.0f),
		glm::vec4(cameraData->Position, 1.0f),
		glm::vec4(surfaceData.albedo, surfaceData.roughness)
	};
	m_pUniformBuffer->LoadData(&data, sizeof(data));

	BindingResource resource[1] =
	{
		{0, ResourceType::UniformBuffer, 1, SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT},
	};
	BufferResourceInfo bufferInfo[] =
	{
		{ m_pUniformBuffer, 0, sizeof(data) },
	};
	RHIUpdateResourceDescriptor updateResourceDescriptor = {};
	{
		updateResourceDescriptor.bindingCount = 1;
		updateResourceDescriptor.pBindingResources = resource;
		updateResourceDescriptor.pGroup = m_pGroup;
		updateResourceDescriptor.bufferResourceCount = 1;
		updateResourceDescriptor.pBufferInfo = bufferInfo;
	}
	context->UpdateUniformResourceToGroup(&updateResourceDescriptor);

	cmd->BeginScopePass("Test");
	{
		RHIGraphicsEncoder* encoder = cmd->GetGraphicsEncoder();
		RHIRenderPassBeginDescriptor renderPassBeginDescriptor = {};
		{
			renderPassBeginDescriptor.renderPass = m_pRenderPass;
			renderPassBeginDescriptor.renderTarget = m_pRenderTargets[RHIContext::g_currentFrame];
		}
		encoder->BeginPass(&renderPassBeginDescriptor);
		//TextureBarrier textureBarriers[] =
		//{
		//	//{ context->GetTexture(ScriptableRenderPipeline::g_currentImage), AttachmentLayout::Undefined, AttachmentLayout::ColorBuffer, 0, ACCESS_COLOR_ATTACHMENT_READ | ACCESS_COLOR_ATTACHMENT_WRITE },
		//	{ m_pDepthTextures[ScriptableRenderPipeline::g_currentImage], AttachmentLayout::Undefined, AttachmentLayout::DepthBuffer, 0, ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_COLOR_ATTACHMENT_WRITE }
		//};
		//RHIBarrierDescriptor barrierDescriptor = {};
		//{
		//	barrierDescriptor.textureCount = 1;
		//	barrierDescriptor.pTextureBarriers = textureBarriers;
		//	barrierDescriptor.srcStage = PIPELINE_STAGE_TOP_OF_PIPE;
		//	barrierDescriptor.dstStage = PIPELINE_STAGE_EARLY_FRAGMENT_TESTS;
		//}
		//encoder->ResourceBarrier(&barrierDescriptor);
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
}
