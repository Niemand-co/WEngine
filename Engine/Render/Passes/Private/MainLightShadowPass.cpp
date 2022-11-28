#include "pch.h"
#include "Render/Passes/Public/MainLightShadowPass.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "RHI/Public/RHIHeads.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Scene/Public/World.h"
#include "Scene/Public/GameObject.h"
#include "Scene/Components/Public/Camera.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "Render/Public/ScriptableRenderer.h"

struct SceneData
{
	glm::mat4 lightSpaceMatrix;
};

struct ObjectData
{
	glm::mat4 modelMatrix;
};

MainLightShadowPass::MainLightShadowPass(ScriptableRenderer* pRenderer)
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
		{ -1, 0, 0, 0, 0, 0 },
		{ 0, PIPELINE_STAGE_EARLY_FRAGMENT_TESTS, ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE, -1, PIPELINE_STAGE_FRAGMENT_SHADER, ACCESS_COLOR_ATTACHMENT_READ | ACCESS_COLOR_ATTACHMENT_WRITE }
	};

	RHIRenderPassDescriptor renderPassDescriptor = {};
	{
		renderPassDescriptor.attachmentCount = 1;
		renderPassDescriptor.pAttachmentDescriptors = attachmentDescriptor;
		renderPassDescriptor.subpassCount = 1;
		renderPassDescriptor.pSubPassDescriptors = &subpassDescriptor;
		renderPassDescriptor.dependencyCount = 2;
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

	pRenderer->SetGlobalTexture(m_pDepthTextureViews);

	m_mainLightCascadedShadowMapRange.resize(m_mainLightCascadedShadowMapNum + 1);
	m_mainLightCascadedShadowMapRange[0] = 0.01f;
	m_mainLightCascadedShadowMapRange[1] = 25.0f;
	m_mainLightCascadedShadowMapRange[2] = 50.0f;
	m_mainLightCascadedShadowMapRange[3] = 75.0f;
	m_mainLightCascadedShadowMapRange[4] = 100.0f;
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
		depthStencilDescriptor.minDepth = 0.0f;
		depthStencilDescriptor.maxDepth = 1.0f;
		depthStencilDescriptor.depthCompareOP = CompareOP::Less;
	}

	RHIBlendDescriptor blendDescriptor = {};
	{
		blendDescriptor.blendEnabled = false;
	}

	m_pSceneDataBuffers.resize(RHIContext::g_maxFrames);
	RHIBufferDescriptor sceneBufferDescriptor = {};
	{
		sceneBufferDescriptor.dataSize = sizeof(SceneData);
		sceneBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pSceneDataBuffers[0] = context->CreateUniformBuffer(&sceneBufferDescriptor);
	m_pSceneDataBuffers[1] = context->CreateUniformBuffer(&sceneBufferDescriptor);
	m_pSceneDataBuffers[2] = context->CreateUniformBuffer(&sceneBufferDescriptor);

	m_pObjectDataBuffers.resize(RHIContext::g_maxFrames);
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
	RHIGroupLayout *layout = context->CreateGroupLayout(&groupLayoutDescriptor);

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
	
	m_pRenderTargets.resize(RHIContext::g_maxFrames);
	for (unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
	{
		RHITextureView *views[] = { m_pDepthTextureViews[i] };
		RHIRenderTargetDescriptor renderTargetDescriptor = {};
		{
			renderTargetDescriptor.width = 512;
			renderTargetDescriptor.height = 512;
			renderTargetDescriptor.pBufferView = views;
			renderTargetDescriptor.renderPass = m_pRenderPass;
			renderTargetDescriptor.bufferCount = 1;
		}
		m_pRenderTargets[i] = RHIContext::GetDevice()->CreateRenderTarget(&renderTargetDescriptor);
	}

	m_pCommandBuffers = context->GetCommandBuffer(RHIContext::g_maxFrames, false);
}

void MainLightShadowPass::Execute(RHIContext *context, CameraData* cameraData)
{
	RHICommandBuffer* cmd = m_pCommandBuffers[RHIContext::g_currentFrame];
	std::vector<glm::mat4> frustum = GetShadowFrustum(cameraData);
	SceneData sceneData =
	{
		frustum[0] *  World::GetWorld()->GetMainLight()->GetGameObject()->GetComponent<Transformer>()->GetWorldToLocalMatrix()
	};
	m_pSceneDataBuffers[RHIContext::g_currentFrame]->LoadData(&sceneData, sizeof(sceneData));

	cmd->BeginScopePass("MainLightShadow", m_pRenderPass, 0, m_pRenderTargets[RHIContext::g_currentImage]);
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
		encoder->SetViewport({ 512, 512, 0, 0 });
		encoder->SetScissor({ 512, 512, 0, 0 });

		unsigned int drawcalls = 0;
		const std::vector<GameObject*>& gameObjects = World::GetWorld()->GetGameObjects();
		for (unsigned int i = 0; i < gameObjects.size(); ++i)
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
		for (unsigned int i = 0; i < gameObjects.size(); ++i)
		{
			MeshFilter *filter = gameObjects[i]->GetComponent<MeshFilter>();
			if(filter == nullptr)
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

void MainLightShadowPass::UpdateRenderTarget(CameraData* cameraData)
{
}

std::vector<glm::mat4> MainLightShadowPass::GetShadowFrustum(CameraData* cameraData)
{
	std::vector<glm::mat4> shadowFrustum(m_mainLightCascadedShadowMapNum);

	Transformer *pTransformer = cameraData->camera->GetGameObject()->GetComponent<Transformer>();
	glm::vec3 center = cameraData->Position;
	glm::vec3 forward = pTransformer->GetForward();
	glm::mat4 lightSpaceMatrix = World::GetWorld()->GetMainLight()->GetGameObject()->GetComponent<Transformer>()->GetWorldToLocalMatrix();
	glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 up = glm::normalize(glm::cross(right, forward));

	for (unsigned int i = 0; i < m_mainLightCascadedShadowMapNum; ++i)
	{
		float half_fov = glm::radians(cameraData->fov / 2.0f);
		float div_aspect = 1.0f / cameraData->aspect;

		float near_height = half_fov * m_mainLightCascadedShadowMapRange[i];
		float near_width = near_height * div_aspect;

		float far_height = half_fov * m_mainLightCascadedShadowMapRange[i + 1u];
		float far_width = far_height * div_aspect;

		glm::vec3 near_center = center + m_mainLightCascadedShadowMapRange[i] * forward;
		glm::vec3 far_center = center + m_mainLightCascadedShadowMapRange[i + 1u] * forward;

		glm::vec3 frustum[8];
		frustum[0] = near_center + right * near_width + up * near_height;
		frustum[1] = near_center + right * near_width - up * near_height;
		frustum[2] = near_center - right * near_width - up * near_height;
		frustum[3] = near_center - right * near_width + up * near_height;

		frustum[4] = far_center + right * far_width + up * far_height;
		frustum[5] = far_center + right * far_width - up * far_height;
		frustum[6] = far_center - right * far_width - up * far_height;
		frustum[7] = far_center - right * far_width + up * far_height;

		float nearP = (std::numeric_limits<float>::max)();
		float farP = (std::numeric_limits<float>::min)();
		float left = (std::numeric_limits<float>::max)();
		float right = (std::numeric_limits<float>::min)();
		float bottom = (std::numeric_limits<float>::max)();
		float top = (std::numeric_limits<float>::min)();
		for (unsigned int j = 0; j < 8; ++j)
		{
			frustum[j] = lightSpaceMatrix * glm::vec4(frustum[j], 1.0f);
			nearP = frustum[j].z < nearP ? frustum[j].z : nearP;
			farP = frustum[j].z > farP ? frustum[j].z : farP;
			left = frustum[j].x < left ? frustum[j].x : left;
			right = frustum[j].x > right ? frustum[j].x : right;
			bottom = frustum[j].y < bottom ? frustum[j].y : bottom;
			top = frustum[j].y > top ? frustum[j].y : top;
		}

		shadowFrustum[i] = glm::ortho(left, right, bottom, top, nearP, farP);
	}

	return shadowFrustum;
}
