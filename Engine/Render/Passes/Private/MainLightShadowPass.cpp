#include "pch.h"
//#include "Render/Passes/Public/MainLightShadowPass.h"
//#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
//#include "RHI/Public/RHIHeads.h"
//#include "Render/Mesh/Public/Mesh.h"
//#include "Render/Mesh/Public/Vertex.h"
//#include "Scene/Public/World.h"
//#include "Scene/Public/GameObject.h"
//#include "Scene/Components/Public/CameraComponent.h"
//#include "Scene/Components/Public/LightComponent.h"
//#include "Scene/Components/Public/TransformComponent.h"
//#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
//#include "Render/Public/ScriptableRenderer.h"
//
//struct SceneData
//{
//	glm::mat4 lightSpaceMatrix;
//};
//
//struct ObjectData
//{
//	glm::mat4 modelMatrix;
//};
//
//MainLightShadowPass::MainLightShadowPass(ScriptableRenderer* pRenderer)
//	: ScriptableRenderPass(pRenderer)
//{
//	RHIAttachmentDescriptor attachmentDescriptor[] = 
//	{
//		{ Format::D32_SFloat, 1, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLayout::DepthBuffer, AttachmentLayout::DepthBuffer },
//	};
//
//	SubPassAttachment depthAttachment = { 0, AttachmentLayout::DepthBuffer };
//
//	RHISubPassDescriptor subpassDescriptor = {};
//	{
//		subpassDescriptor.colorAttachmentCount = 0;
//		subpassDescriptor.pColorAttachments = nullptr;
//		subpassDescriptor.pDepthStencilAttachment = &depthAttachment;
//	}
//
//	RHISubPassDependencyDescriptor dependencyDescriptor[] = 
//	{
//		{ -1, 0, 0, 0, 0, 0 },
//		{ 0, PIPELINE_STAGE_EARLY_FRAGMENT_TESTS, ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE, -1, PIPELINE_STAGE_FRAGMENT_SHADER, ACCESS_COLOR_ATTACHMENT_READ | ACCESS_COLOR_ATTACHMENT_WRITE }
//	};
//
//	RHIRenderPassDescriptor renderPassDescriptor = {};
//	{
//		renderPassDescriptor.attachmentCount = 1;
//		renderPassDescriptor.pAttachmentDescriptors = attachmentDescriptor;
//		renderPassDescriptor.subpassCount = 1;
//		renderPassDescriptor.pSubPassDescriptors = &subpassDescriptor;
//		renderPassDescriptor.dependencyCount = 2;
//		renderPassDescriptor.pDependencyDescriptors = dependencyDescriptor;
//	}
//	m_pRenderPass = RHIContext::GetDevice()->CreateRenderPass(&renderPassDescriptor);
//
//	RHIRenderTargetDescriptor renderTargetDescriptor = {};
//	{
//		renderTargetDescriptor.width = 1024;
//		renderTargetDescriptor.height = 1024;
//		renderTargetDescriptor.renderPass = m_pRenderPass;
//		renderTargetDescriptor.bufferCount = 1;
//	}
//
//}
//
//MainLightShadowPass::~MainLightShadowPass()
//{
//}
//
//void MainLightShadowPass::Setup(RHIContext *context, CameraInfo *cameraData)
//{
//	ShaderCodeBlob* vertBlob = new ShaderCodeBlob("../assets/DepthOnlyVert.spv");
//	RHIShaderDescriptor vertShaderDescriptor = {};
//	{
//		vertShaderDescriptor.entryName = "vert";
//		vertShaderDescriptor.shaderStage = SHADER_STAGE_VERTEX;
//		vertShaderDescriptor.pCode = vertBlob->GetCode();
//		vertShaderDescriptor.codeSize = vertBlob->GetSize();
//	}
//	RHIShader* vertShader = m_pDevice->CreateShader(&vertShaderDescriptor);
//
//	ShaderCodeBlob* fragBlob = new ShaderCodeBlob("../assets/DepthOnlyFrag.spv");
//	RHIShaderDescriptor fragShaderDescriptor = {};
//	{
//		fragShaderDescriptor.entryName = "frag";
//		fragShaderDescriptor.shaderStage = SHADER_STAGE_FRAGMENT;
//		fragShaderDescriptor.pCode = fragBlob->GetCode();
//		fragShaderDescriptor.codeSize = fragBlob->GetSize();
//	}
//	RHIShader* fragShader = m_pDevice->CreateShader(&fragShaderDescriptor);
//
//	RHIShader* shaders[] = { vertShader, fragShader };
//
//	RHIVertexInputDescriptor vertexInputDescriptor = Vertex::GetVertexInputDescriptor();
//
//	RHIRasterizationStateDescriptor rasterizerStateDescriptor = {};
//	{
//		rasterizerStateDescriptor.lineWidth = 1.0f;
//		rasterizerStateDescriptor.polygonMode = PolygonMode::Triangle;
//	}
//
//	RHIDepthStencilDescriptor depthStencilDescriptor = {};
//	{
//		depthStencilDescriptor.depthWriteEnabled = true;
//		depthStencilDescriptor.depthTestEnabled = true;
//		depthStencilDescriptor.depthCompareOP = CompareOP::LE;
//		depthStencilDescriptor.depthBoundsTest = false;
//		depthStencilDescriptor.maxDepth = 1.0f;
//		depthStencilDescriptor.minDepth = 0.0f;
//	}
//
//	RHIBlendDescriptor blendDescriptor = {};
//	{
//		blendDescriptor.blendEnabled = false;
//	}
//
//	m_pSceneDataBuffers.Resize(RHIContext::g_maxFrames * 4);
//	RHIBufferDescriptor sceneBufferDescriptor = {};
//	{
//		sceneBufferDescriptor.dataSize = sizeof(SceneData);
//		sceneBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
//	}
//	for (unsigned int i = 0; i < m_pSceneDataBuffers.Size(); ++i)
//	{
//		m_pSceneDataBuffers[i] = context->CreateUniformBuffer(&sceneBufferDescriptor);
//	}
//
//	m_pObjectDataBuffers.Resize(RHIContext::g_maxFrames);
//	RHIBufferDescriptor objectBufferDescriptor = {};
//	{
//		objectBufferDescriptor.dataSize = sizeof(ObjectData);
//		objectBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE;
//		objectBufferDescriptor.isDynamic = true;
//		objectBufferDescriptor.count = 10;
//	}
//	m_pObjectDataBuffers[0] = context->CreateUniformBuffer(&objectBufferDescriptor);
//	m_pObjectDataBuffers[1] = context->CreateUniformBuffer(&objectBufferDescriptor);
//	m_pObjectDataBuffers[2] = context->CreateUniformBuffer(&objectBufferDescriptor);
//
//	BindingResource resource[] = 
//	{
//		{ 0, ResourceType::UniformBuffer, 1, SHADER_STAGE_VERTEX },
//		{ 1, ResourceType::DynamicUniformBuffer, 1, SHADER_STAGE_VERTEX },
//	};
//	RHIGroupLayoutDescriptor groupLayoutDescriptor = {};
//	{
//		groupLayoutDescriptor.bindingCount = 2;
//		groupLayoutDescriptor.pBindingResources = resource;
//	}
//	RHIGroupLayout *layout = context->CreateGroupLayout(&groupLayoutDescriptor);
//
//	RHIPipelineResourceLayoutDescriptor pipelineResourceLayoutDescriptor = {};
//	{
//		pipelineResourceLayoutDescriptor.groupLayoutCount = 1;
//		pipelineResourceLayoutDescriptor.pGroupLayout = layout;
//	}
//
//	m_pPipelineLayout = context->CreatePipelineResourceLayout(&pipelineResourceLayoutDescriptor);
//
//	RHIViewportDescriptor viewportDescriptor = {};
//	{
//		viewportDescriptor.x = 0.0f;
//		viewportDescriptor.y = 0.0f;
//		viewportDescriptor.width = 1024.0f;
//		viewportDescriptor.height = 1024.0f;
//	}
//
//	RHIScissorDescriptor scissorDescriptor = {};
//	{
//		scissorDescriptor.offsetX = 0;
//		scissorDescriptor.offsetY = 0;
//		scissorDescriptor.width = 1024;
//		scissorDescriptor.height = 1024;
//	}
//
//	RHIPipelineStateObjectDescriptor psoDescriptor = {};
//	{
//		psoDescriptor.shaderCount = 2;
//		psoDescriptor.pShader = shaders;
//		psoDescriptor.vertexDescriptor = &vertexInputDescriptor;
//		psoDescriptor.rasterizationStateDescriptor = &rasterizerStateDescriptor;
//		psoDescriptor.depthStencilDescriptor = &depthStencilDescriptor;
//		psoDescriptor.blendDescriptor = &blendDescriptor;
//		psoDescriptor.subpass = 0;
//		psoDescriptor.pipelineResourceLayout = m_pPipelineLayout;
//		psoDescriptor.renderPass = m_pRenderPass;
//		psoDescriptor.scissorCount = 1;
//		psoDescriptor.pScissors = context->CreateScissor(&scissorDescriptor);
//		psoDescriptor.viewportCount = 1;
//		psoDescriptor.pViewports = context->CreateViewport(&viewportDescriptor);
//	}
//	m_pPSO = context->CreatePSO(&psoDescriptor);
//
//	RHIGroupDescriptor groupDescriptor = {};
//	{
//		groupDescriptor.count = RHIContext::g_maxFrames * 4;
//		groupDescriptor.pGroupLayout = layout;
//	}
//	m_pDataGroup = context->CreateResourceGroup(&groupDescriptor);
//
//	for (unsigned int i = 0; i < 4; ++i)
//	{
//		for (unsigned int j = 0; j < RHIContext::g_maxFrames; ++j)
//		{
//			m_pSceneDataBuffers[i + j * 4]->SetDataSize(sizeof(SceneData));
//			m_pObjectDataBuffers[j]->SetDataSize(sizeof(ObjectData));
//			m_pObjectDataBuffers[j]->Resize(2);
//			RHIBindingDescriptor bindingDescriptors[] = 
//			{
//				{ m_pSceneDataBuffers[i + j * 4]->Size(), m_pSceneDataBuffers[i + j * 4]->GetBufferInfo() },
//				{ m_pObjectDataBuffers[j]->Size(), m_pObjectDataBuffers[j]->GetBufferInfo() },
//			};
//			RHIUpdateResourceDescriptor updateResourceDescriptor = {};
//			{
//				updateResourceDescriptor.bindingCount = 2;
//				updateResourceDescriptor.pBindingResources = resource;
//				updateResourceDescriptor.pGroup = m_pDataGroup[i + j * 4];
//				updateResourceDescriptor.pBindingDescriptors = bindingDescriptors;
//			}
//			context->UpdateUniformResourceToGroup(&updateResourceDescriptor);
//		}
//	}
//	
//	const WEngine::WArray<RHITextureView*>& depthTextureViews = GWorld::GetWorld()->GetMainLight()->GetDepthTexture();
//	m_pRenderTargets.Resize(depthTextureViews.Size());
//	for (unsigned int i = 0; i < depthTextureViews.Size(); ++i)
//	{
//		RHITextureView *views[] = { depthTextureViews[i] };
//		RHIRenderTargetDescriptor renderTargetDescriptor = {};
//		{
//			renderTargetDescriptor.width = 1024;
//			renderTargetDescriptor.height = 1024;
//			renderTargetDescriptor.pBufferView = views;
//			renderTargetDescriptor.renderPass = m_pRenderPass;
//			renderTargetDescriptor.bufferCount = 1;
//		}
//		m_pRenderTargets[i] = RHIContext::GetDevice()->CreateRenderTarget(&renderTargetDescriptor);
//	}
//
//	m_pCommandBuffers = context->GetCommandBuffer(RHIContext::g_maxFrames, false);
//}
//
//void MainLightShadowPass::Execute(RHIContext *context, CameraInfo* cameraData)
//{
//	RHICommandBuffer* cmd = m_pCommandBuffers[RHIContext::g_currentFrame];
//	LightComponent *mainLight = GWorld::GetWorld()->GetMainLight();
//	WEngine::WArray<glm::mat4> frustum = mainLight->GetShadowFrustum();
//	WEngine::WArray<float> splices = mainLight->GetSplices();
//
//
//	cmd->BeginScopePass("MainLightShadow", m_pRenderPass, 0, m_pRenderTargets[RHIContext::g_currentImage]);
//	{
//		RHIGraphicsEncoder* encoder = cmd->GetGraphicsEncoder();
//		ClearValue values[]{ {glm::vec4(1.f, 0.f, 0.f, 0.f), false } };
//
//		encoder->SetPipeline(m_pPSO);
//		//encoder->SetViewport({ 1024, 1024, 0, 0 });
//		//encoder->SetScissor({ 1024, 1024, 0, 0 });
//		for (unsigned int levelIndex = 0; levelIndex < 4; ++levelIndex)
//		{
//			SceneData sceneData =
//			{
//				frustum[levelIndex],
//			};
//			m_pSceneDataBuffers[levelIndex + RHIContext::g_currentFrame * 4]->LoadData(&sceneData, sizeof(sceneData));
//
//			RHIRenderPassBeginDescriptor renderpassBeginDescriptor = {};
//			{
//				renderpassBeginDescriptor.renderPass = m_pRenderPass;
//				renderpassBeginDescriptor.renderTarget = m_pRenderTargets[RHIContext::g_currentFrame * 4 + levelIndex];
//				renderpassBeginDescriptor.clearCount = 1;
//				renderpassBeginDescriptor.pClearValues = values;
//			}
//			encoder->BeginPass(&renderpassBeginDescriptor);
//
//			unsigned int drawcalls = 0;
//			const WEngine::WArray<GameObject*>& gameObjects = GWorld::GetWorld()->GetGameObjects();
//			for (unsigned int i = 0; i < gameObjects.Size(); ++i)
//			{
//				MeshFilter* filter = gameObjects[i]->GetComponent<MeshFilter>();
//				if (filter == nullptr)
//					continue;
//
//				ObjectData objectData = { gameObjects[i]->GetComponent<TransformComponent>()->GetLocalToWorldMatrix() };
//				m_pObjectDataBuffers[RHIContext::g_currentFrame]->LoadData(&objectData, sizeof(objectData), drawcalls);
//				++drawcalls;
//			}
//			m_pObjectDataBuffers[RHIContext::g_currentFrame]->Flush(drawcalls);
//
//			drawcalls = 0;
//			for (unsigned int i = 0; i < gameObjects.Size(); ++i)
//			{
//				MeshFilter *filter = gameObjects[i]->GetComponent<MeshFilter>();
//				if(filter == nullptr)
//					continue;
//
//				StaticMesh* pMesh = filter->GetStaticMesh();
//				encoder->BindVertexBuffer(pMesh->GetVertexBuffer());
//				encoder->BindIndexBuffer(pMesh->GetIndexBuffer());
//				encoder->BindGroups(1, m_pDataGroup[levelIndex + RHIContext::g_currentFrame * 4], m_pPipelineLayout, 1, &drawcalls);
//				encoder->DrawIndexed(pMesh->m_indexCount, 0);
//				drawcalls += m_pObjectDataBuffers[RHIContext::g_currentFrame]->Alignment();
//			}
//			encoder->EndPass();
//		}
//		encoder->~RHIGraphicsEncoder();
//		NormalAllocator::Get()->Deallocate(encoder);
//	}
//	cmd->EndScopePass();
//	context->ExecuteCommandBuffer(cmd);
//}
//
//void MainLightShadowPass::UpdateRenderTarget(CameraInfo* cameraData)
//{
//}