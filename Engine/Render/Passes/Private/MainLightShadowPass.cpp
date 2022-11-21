#include "pch.h"
#include "Render/Passes/Public/MainLightShadowPass.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "RHI/Public/RHIHeads.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Scene/Public/World.h"
#include "Scene/Components/Public/Camera.h"

struct SceneData
{
	glm::vec4 lightSpaceMatrix;
};

struct ObjectData
{
	glm::vec4 modelMatrix;
};

MainLightShadowPass::MainLightShadowPass()
{
	RHIAttachmentDescriptor attachmentDescriptor[] = 
	{
		{ Format::D16_Unorm, 1, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLayout::DepthBuffer, AttachmentLayout::DepthBuffer },
	};

	SubPassAttachment depthAttachment = { 0, AttachmentLayout::DepthBuffer };

	RHISubPassDescriptor subpassDescriptor = {};
	{
		subpassDescriptor.colorAttachmentCount = 0;
		subpassDescriptor.pColorAttachments = nullptr;
		subpassDescriptor.pDepthStencilAttachment = &depthAttachment;
		subpassDescriptor.dependedPass = -1;
		subpassDescriptor.dependedStage = 0;
		subpassDescriptor.dependedAccess = 0;
		subpassDescriptor.waitingAccess = ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE;
	}

	RHIRenderPassDescriptor renderPassDescriptor = {};
	{
		renderPassDescriptor.attachmentCount = 1;
		renderPassDescriptor.pAttachmentDescriptors = attachmentDescriptor;
		renderPassDescriptor.subpassCount = 1;
		renderPassDescriptor.pSubPassDescriptors = &subpassDescriptor;
	}
	m_pRenderPass = RHIContext::GetDevice()->CreateRenderPass(&renderPassDescriptor);

	RHITextureDescriptor textureDescriptor = {};
	{
		textureDescriptor.format = Format::D16_Unorm;
		textureDescriptor.width = 512;
		textureDescriptor.height = 512;
		textureDescriptor.layerCount = 1;
		textureDescriptor.mipCount = 1;
		textureDescriptor.usage = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT | IMAGE_USAGE_SAMPLED;
	}

	RHITextureViewDescriptor textureViewDescriptor = {};
	{
		textureViewDescriptor.format = Format::D16_Unorm;
		textureViewDescriptor.dimension = Dimension::Texture2D;
		textureViewDescriptor.arrayLayerCount = 1;
		textureViewDescriptor.baseArrayLayer = 0;
		textureViewDescriptor.mipCount = 1;
		textureViewDescriptor.baseMipLevel = 0;
		textureViewDescriptor.imageAspect = IMAGE_ASPECT_DEPTH;
	}

	RHIRenderTargetDescriptor renderTargetDescriptor = {};
	{
		renderTargetDescriptor.width = 512;
		renderTargetDescriptor.height = 512;
		renderTargetDescriptor.renderPass = m_pRenderPass;
		renderTargetDescriptor.bufferCount = 1;
	}

	m_pDepthTextures.resize(RHIContext::g_maxFrames);
	m_pDepthTextureViews.resize(RHIContext::g_maxFrames);
	m_pRenderTargets.resize(RHIContext::g_maxFrames);
	for (unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
	{
		m_pDepthTextures[i] = RHIContext::GetDevice()->CreateTexture(&textureDescriptor);
		TextureBarrier barrier = { m_pDepthTextures[i], AttachmentLayout::Undefined, AttachmentLayout::DepthBuffer, 0, ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE, IMAGE_ASPECT_DEPTH };
		RHIBarrierDescriptor barrierDescriptor = {};
		{
			barrierDescriptor.textureCount = 1;
			barrierDescriptor.pTextureBarriers = &barrier;
			barrierDescriptor.srcStage = PIPELINE_STAGE_TOP_OF_PIPE;
			barrierDescriptor.dstStage = PIPELINE_STAGE_EARLY_FRAGMENT_TESTS;
		}
		RHIContext::GetContext()->ResourceBarrier(&barrierDescriptor);
		m_pDepthTextureViews[i] = m_pDepthTextures[i]->CreateTextureView(&textureViewDescriptor);
		{
			renderTargetDescriptor.pBufferView = &m_pDepthTextureViews[i];
		}
		m_pRenderTargets[i] = RHIContext::GetDevice()->CreateRenderTarget(&renderTargetDescriptor);
	}
}

MainLightShadowPass::~MainLightShadowPass()
{
}

void MainLightShadowPass::Setup(RHIContext *context, CameraData *cameraData)
{
	ShaderCodeBlob* vertBlob = new ShaderCodeBlob("../assets/DepthOnlyVert.spv");
	RHIShaderDescriptor vertShaderDescriptor = {};
	{
		vertShaderDescriptor.entryName = "vert";
		vertShaderDescriptor.shaderStage = SHADER_STAGE_VERTEX;
		vertShaderDescriptor.pCode = vertBlob->GetCode();
		vertShaderDescriptor.codeSize = vertBlob->GetSize();
	}
	RHIShader* vertShader = m_pDevice->CreateShader(&vertShaderDescriptor);

	ShaderCodeBlob* fragBlob = new ShaderCodeBlob("../assets/DepthOnlyFrag.spv");
	RHIShaderDescriptor fragShaderDescriptor = {};
	{
		fragShaderDescriptor.entryName = "frag";
		fragShaderDescriptor.shaderStage = SHADER_STAGE_FRAGMENT;
		fragShaderDescriptor.pCode = fragBlob->GetCode();
		fragShaderDescriptor.codeSize = fragBlob->GetSize();
	}
	RHIShader* fragShader = m_pDevice->CreateShader(&fragShaderDescriptor);

	RHIShader* shaders[] = { vertShader, fragShader };

	RHIVertexInputDescriptor vertexInputDescriptor = Vertex::GetVertexInputDescriptor();

	RHIRasterizationStateDescriptor rasterizerStateDescriptor = {};
	{
		rasterizerStateDescriptor.lineWidth = 1.0f;
		rasterizerStateDescriptor.polygonMode = PolygonMode::Triangle;
	}

	RHIDepthStencilDescriptor depthStencilDescriptor = {};
	{
		depthStencilDescriptor.stencilTestEnabled = false;
		depthStencilDescriptor.depthTestEnabled = true;
		depthStencilDescriptor.depthWriteEnabled = true;
		depthStencilDescriptor.minDepth = 1.0f;
		depthStencilDescriptor.maxDepth = 0.0f;
	}

	RHIBlendDescriptor blendDescriptor = {};
	{
		blendDescriptor.blendEnabled = false;
	}

	RHIBufferDescriptor sceneBufferDescriptor = {};
	{
		sceneBufferDescriptor.dataSize = sizeof(SceneData);
		sceneBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pSceneDataBuffers[0] = context->CreateUniformBuffer(&sceneBufferDescriptor);
	m_pSceneDataBuffers[1] = context->CreateUniformBuffer(&sceneBufferDescriptor);
	m_pSceneDataBuffers[2] = context->CreateUniformBuffer(&sceneBufferDescriptor);

	dynamicBufferAlignment = sizeof(ObjectData);
	size_t minUBOAligment = RHIContext::GetGPU()->GetFeature().minUBOAlignment;
	if(minUBOAligment > 0)
		dynamicBufferAlignment = (dynamicBufferAlignment + minUBOAligment - 1) & ~(minUBOAligment - 1);

	RHIBufferDescriptor objectBufferDescriptor = {};
	{
		objectBufferDescriptor.dataSize = World::GetWorld()->GetGameObjects().size() * dynamicBufferAlignment;
		objectBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pObjectDataBuffers[0] = context->CreateUniformBuffer(&objectBufferDescriptor);
	m_pObjectDataBuffers[1] = context->CreateUniformBuffer(&objectBufferDescriptor);
	m_pObjectDataBuffers[2] = context->CreateUniformBuffer(&objectBufferDescriptor);

	BindingResource resource[] = 
	{
		{ 0, ResourceType::UniformBuffer, 1, SHADER_STAGE_VERTEX },
		{ 1, ResourceType::UniformBuffer, 1, SHADER_STAGE_VERTEX },
	};
	RHIGroupLayout layout = { 2u, resource };

	RHIPipelineResourceLayoutDescriptor pipelineResourceLayoutDescriptor = {};
	{
		pipelineResourceLayoutDescriptor.groupLayoutCount = 1;
		pipelineResourceLayoutDescriptor.pGroupLayout = &layout;
	}

	RHIPipelineStateObjectDescriptor psoDescriptor = {};
	{
		psoDescriptor.shaderCount = 2;
		psoDescriptor.pShader = shaders;
		psoDescriptor.vertexDescriptor = &vertexInputDescriptor;
		psoDescriptor.rasterizationStateDescriptor = &rasterizerStateDescriptor;
		psoDescriptor.depthStencilDescriptor = &depthStencilDescriptor;
		psoDescriptor.blendDescriptor = &blendDescriptor;
		psoDescriptor.subpass = 0;
		psoDescriptor.pipelineResourceLayout = context->CreatePipelineResourceLayout(&pipelineResourceLayoutDescriptor);
		psoDescriptor.renderPass = m_pRenderPass;
	}
	m_pPSO = context->CreatePSO(&psoDescriptor);

	RHIGroupDescriptor groupDescriptor = {};
	{
		groupDescriptor.count = RHIContext::g_maxFrames;
		groupDescriptor.pGroupLayout = &layout;
	}
	m_pDataGroup = context->CreateResourceGroup(&groupDescriptor);

	for (unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
	{
		//BufferResourceInfo bufferInfo[][] = 
		//{
		//	{
		//		{ m_pObjectDataBuffers[i], 0, sizeof(ObjectData) },
		//		{ m_pObjectDataBuffers[i], dynamicBufferAlignment, sizeof(ObjectData) },
		//	}
		//	
		//};
		RHIUpdateResourceDescriptor updateResourceDescriptor = {};
		{
			updateResourceDescriptor.bindingCount = 2;
			updateResourceDescriptor.pBindingResources = resource;
			updateResourceDescriptor.pGroup = m_pDataGroup[i];
		}
		context->UpdateUniformResourceToGroup(&updateResourceDescriptor);
	}
	
}

void MainLightShadowPass::Execute(RHIContext *context, CameraData* cameraData)
{
	
}

void MainLightShadowPass::UpdateRenderTarget(CameraData* cameraData)
{
}
