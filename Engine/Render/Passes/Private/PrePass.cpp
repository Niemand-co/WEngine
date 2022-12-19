#include "pch.h"
#include "Render/Passes/Public/PrePass.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "RHI/Public/RHIHeads.h"
#include "Scene/Public/GameObject.h"
#include "Scene/Public/World.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Render/Mesh/Public/Mesh.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "Editor/Public/Screen.h"

struct SceneData
{
	glm::mat4 lightSpaceMatrix;
};

struct ObjectData
{
	glm::mat4 modelMatrix;
};

PrePass::PrePass(ScriptableRenderer* pRenderer)
	: ScriptableRenderPass(pRenderer)
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
	}

	RHISubPassDependencyDescriptor dependencyDescriptor[] = 
	{
		{ -1, 0, 0, 0, 0, ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE }
	};

	RHIRenderPassDescriptor renderPassDescriptor = {};
	{
		renderPassDescriptor.attachmentCount = 1;
		renderPassDescriptor.pAttachmentDescriptors = attachmentDescriptor;
		renderPassDescriptor.subpassCount = 1;
		renderPassDescriptor.pSubPassDescriptors = &subpassDescriptor;
		renderPassDescriptor.dependencyCount = 1;
		renderPassDescriptor.pDependencyDescriptors = dependencyDescriptor;
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
		textureDescriptor.layout = AttachmentLayout::Undefined;
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
		renderTargetDescriptor.width = WEngine::Screen::GetWidth();
		renderTargetDescriptor.height = WEngine::Screen::GetHeight();
		renderTargetDescriptor.renderPass = m_pRenderPass;
		renderTargetDescriptor.bufferCount = 1;
	}

	m_pDepthTextures.Resize(RHIContext::g_maxFrames);
	m_pDepthTextureViews.Resize(RHIContext::g_maxFrames);
	m_pRenderTargets.Resize(RHIContext::g_maxFrames);
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

PrePass::~PrePass()
{
}

void PrePass::Setup(RHIContext* context, CameraData* cameraData)
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
		depthStencilDescriptor.minDepth = 0.0f;
		depthStencilDescriptor.maxDepth = 1.0f;
		depthStencilDescriptor.depthCompareOP = CompareOP::Less;
	}

	RHIBlendDescriptor blendDescriptor = {};
	{
		blendDescriptor.blendEnabled = false;
	}

	m_pSceneDataBuffers.Resize(RHIContext::g_maxFrames);
	RHIBufferDescriptor sceneBufferDescriptor = {};
	{
		sceneBufferDescriptor.dataSize = sizeof(SceneData);
		sceneBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pSceneDataBuffers[0] = context->CreateUniformBuffer(&sceneBufferDescriptor);
	m_pSceneDataBuffers[1] = context->CreateUniformBuffer(&sceneBufferDescriptor);
	m_pSceneDataBuffers[2] = context->CreateUniformBuffer(&sceneBufferDescriptor);

	m_pObjectDataBuffers.Resize(RHIContext::g_maxFrames);
	RHIBufferDescriptor objectBufferDescriptor = {};
	{
		objectBufferDescriptor.dataSize = sizeof(ObjectData);
		objectBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE;
		objectBufferDescriptor.isDynamic = true;
		objectBufferDescriptor.count = 10;
	}
	m_pObjectDataBuffers[0] = context->CreateUniformBuffer(&objectBufferDescriptor);
	m_pObjectDataBuffers[1] = context->CreateUniformBuffer(&objectBufferDescriptor);
	m_pObjectDataBuffers[2] = context->CreateUniformBuffer(&objectBufferDescriptor);

	BindingResource resource[] =
	{
		{ 0, ResourceType::UniformBuffer, 1, SHADER_STAGE_VERTEX },
		{ 1, ResourceType::DynamicUniformBuffer, 1, SHADER_STAGE_VERTEX },
	};
	RHIGroupLayoutDescriptor groupLayoutDescriptor = {};
	{
		groupLayoutDescriptor.bindingCount = 2;
		groupLayoutDescriptor.pBindingResources = resource;
	}
	RHIGroupLayout* layout = context->CreateGroupLayout(&groupLayoutDescriptor);

	RHIPipelineResourceLayoutDescriptor pipelineResourceLayoutDescriptor = {};
	{
		pipelineResourceLayoutDescriptor.groupLayoutCount = 1;
		pipelineResourceLayoutDescriptor.pGroupLayout = layout;
	}

	m_pPipelineLayout = context->CreatePipelineResourceLayout(&pipelineResourceLayoutDescriptor);

	RHIPipelineStateObjectDescriptor psoDescriptor = {};
	{
		psoDescriptor.shaderCount = 2;
		psoDescriptor.pShader = shaders;
		psoDescriptor.vertexDescriptor = &vertexInputDescriptor;
		psoDescriptor.rasterizationStateDescriptor = &rasterizerStateDescriptor;
		psoDescriptor.depthStencilDescriptor = &depthStencilDescriptor;
		psoDescriptor.blendDescriptor = &blendDescriptor;
		psoDescriptor.subpass = 0;
		psoDescriptor.pipelineResourceLayout = m_pPipelineLayout;
		psoDescriptor.renderPass = m_pRenderPass;
	}
	m_pPSO = context->CreatePSO(&psoDescriptor);

	RHIGroupDescriptor groupDescriptor = {};
	{
		groupDescriptor.count = RHIContext::g_maxFrames;
		groupDescriptor.pGroupLayout = layout;
	}
	m_pDataGroup = context->CreateResourceGroup(&groupDescriptor);

	for (unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
	{
		m_pSceneDataBuffers[i]->SetDataSize(sizeof(SceneData));
		m_pObjectDataBuffers[i]->SetDataSize(sizeof(ObjectData));
		m_pObjectDataBuffers[i]->Resize(2);
		RHIBindingDescriptor bindingDescriptors[] =
		{
			{ m_pSceneDataBuffers[i]->Size(), m_pSceneDataBuffers[i]->GetBufferInfo() },
			{ m_pObjectDataBuffers[i]->Size(), m_pObjectDataBuffers[i]->GetBufferInfo() },
		};
		RHIUpdateResourceDescriptor updateResourceDescriptor = {};
		{
			updateResourceDescriptor.bindingCount = 2;
			updateResourceDescriptor.pBindingResources = resource;
			updateResourceDescriptor.pGroup = m_pDataGroup[i];
			updateResourceDescriptor.pBindingDescriptors = bindingDescriptors;
		}
		context->UpdateUniformResourceToGroup(&updateResourceDescriptor);
	}

	m_pRenderTargets.Resize(RHIContext::g_maxFrames);
	for (unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
	{
		RHITextureView* views[] = { m_pDepthTextureViews[i] };
		RHIRenderTargetDescriptor renderTargetDescriptor = {};
		{
			renderTargetDescriptor.width = WEngine::Screen::GetWidth();
			renderTargetDescriptor.height = WEngine::Screen::GetHeight();
			renderTargetDescriptor.pBufferView = views;
			renderTargetDescriptor.renderPass = m_pRenderPass;
			renderTargetDescriptor.bufferCount = 1;
		}
		m_pRenderTargets[i] = RHIContext::GetDevice()->CreateRenderTarget(&renderTargetDescriptor);
	}

	m_pCommandBuffers = context->GetCommandBuffer(RHIContext::g_maxFrames, false);
}

void PrePass::Execute(RHIContext* context, CameraData* cameraData)
{
	RHICommandBuffer* cmd = m_pCommandBuffers[RHIContext::g_currentFrame];

	SceneData sceneData =
	{
		World::GetWorld()->GetMainLight()->GetGameObject()->GetComponent<Transformer>()->GetWorldToLocalMatrix()
	};
	m_pSceneDataBuffers[RHIContext::g_currentFrame]->LoadData(&sceneData, sizeof(sceneData));

	cmd->BeginScopePass("Skybox", m_pRenderPass, 0, m_pRenderTargets[RHIContext::g_currentImage]);
	{
		RHIGraphicsEncoder* encoder = cmd->GetGraphicsEncoder();

		ClearValue values[]{ {glm::vec4(1.f, 0.f, 0.f, 0.f), false } };
		RHIRenderPassBeginDescriptor renderpassBeginDescriptor = {};
		{
			renderpassBeginDescriptor.renderPass = m_pRenderPass;
			renderpassBeginDescriptor.renderTarget = m_pRenderTargets[RHIContext::g_currentFrame];
			renderpassBeginDescriptor.clearCount = 1;
			renderpassBeginDescriptor.pClearValues = values;
		}
		encoder->BeginPass(&renderpassBeginDescriptor);
		encoder->SetPipeline(m_pPSO);
		encoder->SetViewport({ (float)WEngine::Screen::GetWidth(), (float)WEngine::Screen::GetHeight(), 0, 0 });
		encoder->SetScissor({ WEngine::Screen::GetWidth(), WEngine::Screen::GetHeight(), 0, 0 });

		unsigned int drawcalls = 0;
		const WEngine::WArray<GameObject*>& gameObjects = World::GetWorld()->GetGameObjects();
		for (unsigned int i = 0; i < gameObjects.Size(); ++i)
		{
			MeshFilter* filter = gameObjects[i]->GetComponent<MeshFilter>();
			if (filter == nullptr)
				continue;

			ObjectData objectData = { gameObjects[i]->GetComponent<Transformer>()->GetLocalToWorldMatrix() };
			m_pObjectDataBuffers[RHIContext::g_currentFrame]->LoadData(&objectData, sizeof(objectData), drawcalls);
			++drawcalls;
		}
		m_pObjectDataBuffers[RHIContext::g_currentFrame]->Flush(drawcalls);

		drawcalls = 0;
		for (unsigned int i = 0; i < gameObjects.Size(); ++i)
		{
			MeshFilter* filter = gameObjects[i]->GetComponent<MeshFilter>();
			if (filter == nullptr)
				continue;

			Mesh* pMesh = filter->GetStaticMesh();
			encoder->BindVertexBuffer(pMesh->GetVertexBuffer());
			encoder->BindIndexBuffer(pMesh->GetIndexBuffer());
			encoder->BindGroups(1, m_pDataGroup[RHIContext::g_currentFrame], m_pPipelineLayout, 1, &drawcalls);
			encoder->DrawIndexed(pMesh->m_indexCount, 0);
			drawcalls += m_pObjectDataBuffers[RHIContext::g_currentFrame]->Alignment();
		}
		encoder->EndPass();
		encoder->~RHIGraphicsEncoder();
		WEngine::Allocator::Get()->Deallocate(encoder);
	}
	cmd->EndScopePass();
	context->ExecuteCommandBuffer(cmd);
}

void PrePass::UpdateRenderTarget(CameraData* cameraData)
{
	for (int i = 0; i < 3; ++i)
	{
		delete m_pDepthTextureViews[i];

		WEngine::WArray<RHITextureView*> textureViews = { m_pDepthTextureViews[i] };
		RHIRenderTargetDescriptor renderTargetDescriptor = {};
		{
			renderTargetDescriptor.bufferCount = 2;
			renderTargetDescriptor.pBufferView = textureViews.GetData();
			renderTargetDescriptor.renderPass = m_pRenderPass;
			renderTargetDescriptor.width = WEngine::Screen::GetWidth();
			renderTargetDescriptor.height = WEngine::Screen::GetHeight();
		}
		delete m_pRenderTargets[i];
		m_pRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
	}
}
