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
#include "Scene/Public/World.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Editor/Public/Screen.h"

struct UniformData
{
	glm::mat4 M;
	glm::mat4 VP;
	glm::vec4 lightPos;
	glm::vec4 cameraPos;
	glm::vec4 surfaceData;
};

DrawOpaquePass::DrawOpaquePass()
{
	RHIAttachmentDescriptor attachmentDescriptors[] =
	{
		{ Format::A16R16G16B16_SFloat, 1, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLayout::Undefined, AttachmentLayout::ColorBuffer },
		{ Format::D16_Unorm, 1, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLayout::Undefined, AttachmentLayout::DepthBuffer }
	};
	SubPassAttachment subpassColorAttachment = { 0, AttachmentLayout::ColorBuffer };
	SubPassAttachment subpassDepthAttachment = { 1, AttachmentLayout::DepthBuffer };

	RHISubPassDescriptor subpassDescriptors = {};
	{
		subpassDescriptors.colorAttachmentCount = 1;
		subpassDescriptors.pColorAttachments = &subpassColorAttachment;
		subpassDescriptors.pDepthStencilAttachment = &subpassDepthAttachment;
		subpassDescriptors.dependedStage = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		subpassDescriptors.waitingStage = PIPELINE_STAGE_EARLY_FRAGMENT_TESTS;
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
		depthStencilDescriptor.depthCompareOP = CompareOP::Less;
		depthStencilDescriptor.depthBoundsTest = false;
		depthStencilDescriptor.maxDepth = 1.0f;
		depthStencilDescriptor.minDepth = 0.0f;
	}

	BindingResource resource[1] = 
	{
		{0, ResourceType::DynamicUniformBuffer, 1, SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT},
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
		groupDescriptor.count = RHIContext::g_maxFrames;
	}
	m_pGroup = context->CreateResourceGroup(&groupDescriptor);

	RHIRasterizationStateDescriptor rasterizationStateDescriptor = {};

	RHIVertexInputDescriptor vertexInputDescriptor = Vertex::GetVertexInputDescriptor();
	RHIPipelineStateObjectDescriptor psoDescriptor = {};
	{
		psoDescriptor.renderPass = m_pRenderPass;
		psoDescriptor.subpass = 0;
		psoDescriptor.blendDescriptor = &blendDescriptor;
		psoDescriptor.depthStencilDescriptor = &depthStencilDescriptor;
		psoDescriptor.pShader = shaders.data();
		psoDescriptor.shaderCount = shaders.size();
		psoDescriptor.vertexDescriptor = &vertexInputDescriptor;
		psoDescriptor.pipelineResourceLayout = m_pPipelineResourceLayout;
		psoDescriptor.rasterizationStateDescriptor = &rasterizationStateDescriptor;
	};
	m_pPSO = context->CreatePSO(&psoDescriptor);

	m_pUniformBuffers.resize(RHIContext::g_maxFrames);
	RHIBufferDescriptor uniformBufferDescriptor = {};
	{
		uniformBufferDescriptor.isDynamic = true;
		uniformBufferDescriptor.size = sizeof(UniformData);
		uniformBufferDescriptor.count = 2;
		uniformBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE;
	}
	m_pUniformBuffers[0] = context->CreateUniformBuffer(&uniformBufferDescriptor);
	m_pUniformBuffers[1] = context->CreateUniformBuffer(&uniformBufferDescriptor);
	m_pUniformBuffers[2] = context->CreateUniformBuffer(&uniformBufferDescriptor);

	for (unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
	{
		m_pUniformBuffers[i]->SetDataSize(sizeof(UniformData));
		BufferResourceInfo *bufferInfo[1] = 
		{
			m_pUniformBuffers[i]->GetBufferInfo(),
		};
		unsigned int bufferInfoCount[] = { 2 };
		RHIUpdateResourceDescriptor updateResourceDescriptor = {};
		{
			updateResourceDescriptor.bindingCount = 1;
			updateResourceDescriptor.pBindingResources = resource;
			updateResourceDescriptor.pGroup = m_pGroup[i];
			updateResourceDescriptor.dynamicBufferCount = 1;
			updateResourceDescriptor.bufferResourceCount = bufferInfoCount;
			updateResourceDescriptor.pBufferInfo = bufferInfo;
		}
		context->UpdateUniformResourceToGroup(&updateResourceDescriptor);
	}

	m_pRenderTargets.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		RenderTarget &target = cameraData->camera->GetRenderTarget(i);
		std::vector<RHITextureView*> textureViews = { target.pColorTexture, target.pDepthTexture };
		RHIRenderTargetDescriptor renderTargetDescriptor = {};
		{
			renderTargetDescriptor.bufferCount = 2;
			renderTargetDescriptor.pBufferView = textureViews.data();
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

	const std::vector<GameObject*>& gameObjects = World::GetWorld()->GetGameObjects();

	cmd->BeginScopePass("Test", m_pRenderPass, 0, m_pRenderTargets[RHIContext::g_currentFrame]);
	{
		RHIGraphicsEncoder* encoder = cmd->GetGraphicsEncoder();
		ClearValue values[] { {glm::vec4(1.f, 1.f, 1.f, 1.f), 0.0f, 0 }, { glm::vec4(), 1.0f, 0 } };
		RHIRenderPassBeginDescriptor renderPassBeginDescriptor = {};
		{
			renderPassBeginDescriptor.renderPass = m_pRenderPass;
			renderPassBeginDescriptor.renderTarget = m_pRenderTargets[RHIContext::g_currentFrame];
			renderPassBeginDescriptor.clearCount = 2;
			renderPassBeginDescriptor.pClearValues = values;
		}
		encoder->BeginPass(&renderPassBeginDescriptor);
		encoder->SetPipeline(m_pPSO);
		encoder->SetViewport({(float)WEngine::Screen::GetWidth(), (float)WEngine::Screen::GetHeight(), 0, 0});
		encoder->SetScissor({WEngine::Screen::GetWidth(), WEngine::Screen::GetHeight(), 0, 0});

		int drawcalls = 0;
		for (unsigned int i = 0; i < gameObjects.size(); ++i)
		{
			MeshFilter* filter = gameObjects[i]->GetComponent<MeshFilter>();
			if (filter == nullptr)
				continue;

			SurfaceData surfaceData = gameObjects[i]->GetComponent<Material>()->GetSurfaceData();
			UniformData data =
			{
				gameObjects[i]->GetComponent<Transformer>()->GetLocalToWorldMatrix(),
				cameraData->MatrixVP,
				World::GetWorld()->GetMainLight()->GetGameObject()->GetComponent<Transformer>()->GetRotateMatrix() * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f),
				glm::vec4(cameraData->Position, 1.0f),
				glm::vec4(surfaceData.albedo, surfaceData.roughness)
			};
			m_pUniformBuffers[RHIContext::g_currentFrame]->LoadData(&data, sizeof(data), dynamicAlignment * drawcalls);
			++drawcalls;
		}
		m_pUniformBuffers[RHIContext::g_currentFrame]->Flush(2 * dynamicAlignment);

		drawcalls = 0;
		for (unsigned int i = 0; i < gameObjects.size(); ++i)
		{
			MeshFilter *filter = gameObjects[i]->GetComponent<MeshFilter>();
			if(filter == nullptr)
				continue;
			
			unsigned int offset = (unsigned int)dynamicAlignment * drawcalls;
			Mesh *pMesh = filter->GetStaticMesh();
			encoder->BindVertexBuffer(pMesh->GetVertexBuffer());
			encoder->BindIndexBuffer(pMesh->GetIndexBuffer());
			encoder->BindGroups(1, m_pGroup[RHIContext::g_currentFrame], m_pPipelineResourceLayout, 1, &offset);
			encoder->DrawIndexed(pMesh->m_indexCount, 0);
			drawcalls++;
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
		std::vector<RHITextureView*> textureViews = { cameraData->camera->GetRenderTarget(i).pColorTexture, cameraData->camera->GetRenderTarget(i).pDepthTexture };
		RHIRenderTargetDescriptor renderTargetDescriptor = {};
		{
			renderTargetDescriptor.bufferCount = 2;
			renderTargetDescriptor.pBufferView = textureViews.data();
			renderTargetDescriptor.renderPass = m_pRenderPass;
			renderTargetDescriptor.width = WEngine::Screen::GetWidth();
			renderTargetDescriptor.height = WEngine::Screen::GetHeight();
		}
		delete m_pRenderTargets[i];
		m_pRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
	}
}
