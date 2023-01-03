#include "pch.h"
#include "Render/Passes/Public/DrawOpaquePass.h"
#include "RHI/Public/RHIHeads.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "RHI/Encoder/Public/RHIComputeEncoder.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Render/Public/ScriptableRenderer.h"
#include "Utils/Public/Window.h"
#include "Utils/ImGui/Public/Gui.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Scene/Public/GameObject.h"
#include "Scene/Public/World.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Editor/Public/Screen.h"

struct SceneData
{
	glm::mat4 VP;
	glm::mat4 lightSpaceMatrix[4];
	glm::vec4 splices;
	glm::vec4 lightPos;
	glm::vec4 light;
	glm::vec4 cameraPos;
};

struct ObjectData
{
	glm::mat4 M;
	glm::mat4 InvM;
	glm::vec4 surfaceData;
};

DrawOpaquePass::DrawOpaquePass(ScriptableRenderer* pRenderer)
	: ScriptableRenderPass(pRenderer)
{
	RHIAttachmentDescriptor attachmentDescriptors[] =
	{
		{ Format::A16R16G16B16_SFloat, 1, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLayout::ColorBuffer, AttachmentLayout::ColorBuffer },
		{ Format::D16_Unorm, 1, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLayout::DepthBuffer, AttachmentLayout::DepthBuffer },
	};
	SubPassAttachment subpassColorAttachment = { 0, AttachmentLayout::ColorBuffer };
	SubPassAttachment subpassDepthAttachment = { 1, AttachmentLayout::DepthBuffer };

	RHISubPassDescriptor subpassDescriptor = {};
	{
		subpassDescriptor.colorAttachmentCount = 1;
		subpassDescriptor.pColorAttachments = &subpassColorAttachment;
		subpassDescriptor.pDepthStencilAttachment = &subpassDepthAttachment;
	}

	RHISubPassDependencyDescriptor dependencyDescriptor[] =
	{
		{ -1, PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT, ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE, 0, PIPELINE_STAGE_EARLY_FRAGMENT_TESTS | PIPELINE_STAGE_FRAGMENT_SHADER, ACCESS_COLOR_ATTACHMENT_READ | ACCESS_COLOR_ATTACHMENT_WRITE | ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE },
	};

	RHIRenderPassDescriptor renderPassDescriptor = {};
	{
		renderPassDescriptor.attachmentCount = 2;
		renderPassDescriptor.pAttachmentDescriptors = attachmentDescriptors;
		renderPassDescriptor.subpassCount = 1;
		renderPassDescriptor.pSubPassDescriptors = &subpassDescriptor;
		renderPassDescriptor.dependencyCount = 1;
		renderPassDescriptor.pDependencyDescriptors = dependencyDescriptor;
	}
	m_pRenderPass = m_pDevice->CreateRenderPass(&renderPassDescriptor);
}

DrawOpaquePass::~DrawOpaquePass()
{
}

void DrawOpaquePass::Setup(RHIContext *context, CameraData *cameraData)
{
	ShaderCodeBlob* vertBlob = new ShaderCodeBlob("../assets/OpaqueVert.spv");
	RHIShaderDescriptor vertShaderDescriptor = {};
	{
		vertShaderDescriptor.entryName = "vert";
		vertShaderDescriptor.shaderStage = SHADER_STAGE_VERTEX;
		vertShaderDescriptor.pCode = vertBlob->GetCode();
		vertShaderDescriptor.codeSize = vertBlob->GetSize();
	}
	RHIShader* vertShader = m_pDevice->CreateShader(&vertShaderDescriptor);

	ShaderCodeBlob* fragBlob = new ShaderCodeBlob("../assets/OpaqueFrag.spv");
	RHIShaderDescriptor fragShaderDescriptor = {};
	{
		fragShaderDescriptor.entryName = "frag";
		fragShaderDescriptor.shaderStage = SHADER_STAGE_FRAGMENT;
		fragShaderDescriptor.pCode = fragBlob->GetCode();
		fragShaderDescriptor.codeSize = fragBlob->GetSize();
	}
	RHIShader* fragShader = m_pDevice->CreateShader(&fragShaderDescriptor);

	WEngine::WArray<RHIShader*> shaders = { vertShader, fragShader };
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
		depthStencilDescriptor.depthCompareOP = CompareOP::Less;
		depthStencilDescriptor.depthBoundsTest = false;
		depthStencilDescriptor.maxDepth = 1.0f;
		depthStencilDescriptor.minDepth = 0.0f;
	}

	BindingResource resource[4] = 
	{
		{0, ResourceType::UniformBuffer, 1, SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT},
		{1, ResourceType::DynamicUniformBuffer, 1, SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT},
		{2, ResourceType::Sampler, 1, SHADER_STAGE_FRAGMENT},
		{3, ResourceType::Texture, 4, SHADER_STAGE_FRAGMENT},
	};
	RHIGroupLayoutDescriptor groupLayoutDescriptor = {};
	{
		groupLayoutDescriptor.bindingCount = 4;
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
		groupDescriptor.count = RHIContext::g_maxFrames;
	}
	m_pGroup = context->CreateResourceGroup(&groupDescriptor);

	RHIRasterizationStateDescriptor rasterizationStateDescriptor = {};

	RHIViewportDescriptor viewportDescriptor = {};
	{
		viewportDescriptor.x = 0.0f;
		viewportDescriptor.y = 0.0f;
		viewportDescriptor.width = (float)WEngine::Screen::GetWidth();
		viewportDescriptor.height = (float)WEngine::Screen::GetWidth();
	}

	RHIScissorDescriptor scissorDescriptor = {};
	{
		scissorDescriptor.offsetX = 0;
		scissorDescriptor.offsetY = 0;
		scissorDescriptor.width = WEngine::Screen::GetWidth();
		scissorDescriptor.height = WEngine::Screen::GetWidth();
	}

	RHIVertexInputDescriptor vertexInputDescriptor = Vertex::GetVertexInputDescriptor();
	RHIPipelineStateObjectDescriptor psoDescriptor = {};
	{
		psoDescriptor.renderPass = m_pRenderPass;
		psoDescriptor.subpass = 0;
		psoDescriptor.blendDescriptor = &blendDescriptor;
		psoDescriptor.depthStencilDescriptor = &depthStencilDescriptor;
		psoDescriptor.pShader = shaders.GetData();
		psoDescriptor.shaderCount = shaders.Size();
		psoDescriptor.vertexDescriptor = &vertexInputDescriptor;
		psoDescriptor.pipelineResourceLayout = m_pPipelineResourceLayout;
		psoDescriptor.rasterizationStateDescriptor = &rasterizationStateDescriptor;
		psoDescriptor.scissorCount = 1;
		psoDescriptor.pScissors = context->CreateScissor(&scissorDescriptor);
		psoDescriptor.viewportCount = 1;
		psoDescriptor.pViewports = context->CreateViewport(&viewportDescriptor);
	};
	m_pPSO = context->CreatePSO(&psoDescriptor);

	m_pObjectUniformBuffers.Resize(RHIContext::g_maxFrames);
	RHIBufferDescriptor uniformBufferDescriptor = {};
	{
		uniformBufferDescriptor.isDynamic = true;
		uniformBufferDescriptor.dataSize = sizeof(ObjectData);
		uniformBufferDescriptor.count = 10;
		uniformBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE;
	}
	m_pObjectUniformBuffers[0] = context->CreateUniformBuffer(&uniformBufferDescriptor);
	m_pObjectUniformBuffers[1] = context->CreateUniformBuffer(&uniformBufferDescriptor);
	m_pObjectUniformBuffers[2] = context->CreateUniformBuffer(&uniformBufferDescriptor);

	m_pSceneUniformBuffers.Resize(RHIContext::g_maxFrames);
	{
		uniformBufferDescriptor.isDynamic = false;
		uniformBufferDescriptor.count = 1;
		uniformBufferDescriptor.dataSize = sizeof(SceneData);
		uniformBufferDescriptor.memoryType |= MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pSceneUniformBuffers[0] = context->CreateUniformBuffer(&uniformBufferDescriptor);
	m_pSceneUniformBuffers[1] = context->CreateUniformBuffer(&uniformBufferDescriptor);
	m_pSceneUniformBuffers[2] = context->CreateUniformBuffer(&uniformBufferDescriptor);

	const WEngine::WArray<RHITextureView*>& depthTextures = World::GetWorld()->GetMainLight()->GetDepthTexture();
	for (unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
	{
		m_pObjectUniformBuffers[i]->SetDataSize(sizeof(ObjectData));
		m_pObjectUniformBuffers[i]->Resize(2);
		m_pSceneUniformBuffers[i]->SetDataSize(sizeof(SceneData));
		RHIBindingDescriptor bindingDescriptors[] = 
		{
			{ 1, m_pSceneUniformBuffers[i]->GetBufferInfo() },
			{ m_pObjectUniformBuffers[i]->Size(), m_pObjectUniformBuffers[i]->GetBufferInfo() },
		};
		RHIUpdateResourceDescriptor updateResourceDescriptor = {};
		{
			updateResourceDescriptor.bindingCount = 2;
			updateResourceDescriptor.pBindingResources = resource;
			updateResourceDescriptor.pBindingDescriptors = bindingDescriptors;
			updateResourceDescriptor.pGroup = m_pGroup[i];
		}
		context->UpdateUniformResourceToGroup(&updateResourceDescriptor);

		RHISamplerDescriptor samplerDescriptor = {};
		{
			samplerDescriptor.minFilter = Filter::Linear;
			samplerDescriptor.magFilter = Filter::Linear;
		}
		m_pSampler = RHIContext::GetDevice()->CreateSampler(&samplerDescriptor);;

		TextureResourceInfo samplerInfo[] =
		{
			{ nullptr, m_pSampler, AttachmentLayout::ReadOnlyDepth }
		};
		RHIUpdateResourceDescriptor shadowmapResourceDescriptor = {};
		{
			shadowmapResourceDescriptor.bindingCount = 1;
			shadowmapResourceDescriptor.pBindingResources = resource + 2;
			shadowmapResourceDescriptor.textureResourceCount = 1;
			shadowmapResourceDescriptor.pTextureInfo = samplerInfo;
			shadowmapResourceDescriptor.pGroup = m_pGroup[i];
		}
		context->UpdateTextureResourceToGroup(&shadowmapResourceDescriptor);

		TextureResourceInfo textureInfo[] =
		{
			{ depthTextures[0 + i * 4], nullptr, AttachmentLayout::ReadOnlyDepth },
			{ depthTextures[1 + i * 4], nullptr, AttachmentLayout::ReadOnlyDepth },
			{ depthTextures[2 + i * 4], nullptr, AttachmentLayout::ReadOnlyDepth },
			{ depthTextures[3 + i * 4], nullptr, AttachmentLayout::ReadOnlyDepth },
		};
		{
			shadowmapResourceDescriptor.bindingCount = 1;
			shadowmapResourceDescriptor.pBindingResources = resource + 3;
			shadowmapResourceDescriptor.textureResourceCount = 4;
			shadowmapResourceDescriptor.pTextureInfo = textureInfo;
			shadowmapResourceDescriptor.pGroup = m_pGroup[i];
		}
		context->UpdateTextureResourceToGroup(&shadowmapResourceDescriptor);
	}

	m_pRenderTargets.Resize(3);
	for (int i = 0; i < 3; ++i)
	{
		CameraRenderTarget &target = cameraData->camera->GetRenderTarget(i);
		WEngine::WArray<RHITextureView*> textureViews = { target.pColorTexture, target.pDepthTexture };
		RHIRenderTargetDescriptor renderTargetDescriptor = {};
		{
			renderTargetDescriptor.bufferCount = 2;
			renderTargetDescriptor.pBufferView = textureViews.GetData();
			renderTargetDescriptor.renderPass = m_pRenderPass;
			renderTargetDescriptor.width = WEngine::Screen::GetWidth();
			renderTargetDescriptor.height = WEngine::Screen::GetHeight();
		}
		m_pRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
	}

	m_pCommandBuffers = context->GetCommandBuffer(RHIContext::g_maxFrames, false);
}

void DrawOpaquePass::Execute(RHIContext *context, CameraData *cameraData)
{
	RHICommandBuffer *cmd = m_pCommandBuffers[RHIContext::g_currentFrame];

	const WEngine::WArray<GameObject*>& gameObjects = World::GetWorld()->GetGameObjects();

	cmd->BeginScopePass("Opaque", m_pRenderPass, 0, m_pRenderTargets[RHIContext::g_currentFrame]);
	{
		RHIGraphicsEncoder* encoder = cmd->GetGraphicsEncoder();
		ClearValue values[] { {glm::vec4(1.f, 1.f, 1.f, 1.f), true }, { glm::vec4(1.f, 0.f, 0.f, 0.f), false } };
		RHIRenderPassBeginDescriptor renderPassBeginDescriptor = {};
		{
			renderPassBeginDescriptor.renderPass = m_pRenderPass;
			renderPassBeginDescriptor.renderTarget = m_pRenderTargets[RHIContext::g_currentFrame];
			renderPassBeginDescriptor.clearCount = 2;
			renderPassBeginDescriptor.pClearValues = values;
		}
		encoder->BeginPass(&renderPassBeginDescriptor);
		encoder->SetPipeline(m_pPSO);
		//encoder->SetViewport({(float)WEngine::Screen::GetWidth(), (float)WEngine::Screen::GetHeight(), 0, 0});
		//encoder->SetScissor({WEngine::Screen::GetWidth(), WEngine::Screen::GetHeight(), 0, 0});
		unsigned int drawcalls = 0;
		Light *mainLight = World::GetWorld()->GetMainLight();
		const WEngine::WArray<glm::mat4>& frustum = mainLight->GetShadowFrustum();
		const WEngine::WArray<float>& splices = mainLight->GetSplices();

		SceneData sceneData =
		{
			cameraData->MatrixVP,
			frustum[0],
			frustum[1],
			frustum[2],
			frustum[3],
			glm::vec4(splices[0], splices[1], splices[2], splices[3]),
			mainLight->GetGameObject()->GetComponent<Transformer>()->GetRotateMatrix() * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f),
			glm::vec4(mainLight->GetColor() * mainLight->GetIntensity(), 1.0f),
			glm::vec4(cameraData->Position, 1.0f),
		};
		m_pSceneUniformBuffers[RHIContext::g_currentFrame]->LoadData(&sceneData, sizeof(sceneData));
		for (unsigned int i = 0; i < gameObjects.Size(); ++i)
		{
			MeshFilter* filter = gameObjects[i]->GetComponent<MeshFilter>();
			if (filter == nullptr)
				continue;

			SurfaceData surfaceData = gameObjects[i]->GetComponent<Material>()->GetSurfaceData();
			glm::mat4 M = gameObjects[i]->GetComponent<Transformer>()->GetLocalToWorldMatrix();
			ObjectData objectData = 
			{
				M,
				glm::inverse(M),
				glm::vec4(surfaceData.albedo, surfaceData.roughness)
			};
			m_pObjectUniformBuffers[RHIContext::g_currentFrame]->LoadData(&objectData, sizeof(objectData), drawcalls);
			++drawcalls;
		}
		m_pObjectUniformBuffers[RHIContext::g_currentFrame]->Flush(drawcalls);

		drawcalls = 0;
		for (unsigned int i = 0; i < gameObjects.Size(); ++i)
		{
			MeshFilter *filter = gameObjects[i]->GetComponent<MeshFilter>();
			if(filter == nullptr)
				continue;
			
			Mesh *pMesh = filter->GetStaticMesh();
			encoder->BindVertexBuffer(pMesh->GetVertexBuffer());
			encoder->BindIndexBuffer(pMesh->GetIndexBuffer());
			encoder->BindGroups(1, m_pGroup[RHIContext::g_currentFrame], m_pPipelineResourceLayout, 1, &drawcalls);
			encoder->DrawIndexed(pMesh->m_indexCount, 0);
			drawcalls += m_pObjectUniformBuffers[RHIContext::g_currentFrame]->Alignment();
		}

		encoder->EndPass();
		encoder->~RHIGraphicsEncoder();
		WEngine::Allocator::Get()->Deallocate(encoder);
	}
	cmd->EndScopePass();
	context->ExecuteCommandBuffer(cmd);
}

void DrawOpaquePass::UpdateRenderTarget(CameraData* cameraData)
{
	for (int i = 0; i < 3; ++i)
	{
		WEngine::WArray<RHITextureView*> textureViews = { cameraData->camera->GetRenderTarget(i).pColorTexture, cameraData->camera->GetRenderTarget(i).pDepthTexture };
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
