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

DrawOpaquePass::DrawOpaquePass(RenderPassConfigure* configure)
	: ScriptableRenderPass(configure)
{
	RE_LOG(sizeof(SurfaceData));
}

DrawOpaquePass::~DrawOpaquePass()
{
}

void DrawOpaquePass::Setup(RHIContext *context, CameraData *cameraData)
{
	m_pDepthTextures.resize(3);
	RHITextureDescriptor textureDescriptor = {};
	{
		textureDescriptor.format = Format::D32_SFloat;
		textureDescriptor.height = Window::cur_window->GetHeight();
		textureDescriptor.width = Window::cur_window->GetWidth();
		textureDescriptor.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		textureDescriptor.mipCount = 1;
		textureDescriptor.layout = AttachmentLayout::Undefined;
	}
	m_pDepthTextures[0] = m_pDevice->CreateTexture(&textureDescriptor);
	m_pDepthTextures[1] = m_pDevice->CreateTexture(&textureDescriptor);
	m_pDepthTextures[2] = m_pDevice->CreateTexture(&textureDescriptor);
	//TextureBarrier textureBarrier = { m_pDepthTextures, AttachmentLayout::Undefined, AttachmentLayout::DepthBuffer, 0, ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE };
	//RHIBarrierDescriptor barrierDescriptor = {};
	//{
	//	barrierDescriptor.textureCount = 1;
	//	barrierDescriptor.pTextureBarriers = &textureBarrier;
	//	barrierDescriptor.srcStage = PIPELINE_STAGE_TOP_OF_PIPE;
	//	barrierDescriptor.dstStage = PIPELINE_STAGE_EARLY_FRAGMENT_TESTS;
	//}
	//context->ImageLayoutTransition(&barrierDescriptor);

	RHIAttachmentDescriptor attachmentDescriptors[] =
	{
		{ Format::A16R16G16B16_SFloat, 1, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLoadOP::Clear, AttachmentStoreOP::DontCare, AttachmentLayout::Undefined, AttachmentLayout::Present },
		{ Format::D32_SFloat, 1, AttachmentLoadOP::Clear, AttachmentStoreOP::DontCare, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLayout::Undefined, AttachmentLayout::DepthBuffer }
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
		depthStencilDescriptor.depthWriteEnabled = false;
		depthStencilDescriptor.depthTestEnabled = false;
		depthStencilDescriptor.depthCompareOP = DepthCompareOP::Greater;
		depthStencilDescriptor.depthBoundsTest = false;
		depthStencilDescriptor.maxDepth = 0.0f;
		depthStencilDescriptor.minDepth = 1.0f;
	}

	GameObject *go = GameObject::Find("Cube");
	m_pMesh = static_cast<MeshFilter*>(go->GetComponent<Component::ComponentType::MeshFilter>())->GetStaticMesh();

	BindingResource resource[1] = 
	{
		{0, ResourceType::UniformBuffer, 1, ShaderStage::vertex},
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

	SurfaceData surfaceData = static_cast<Material*>(go->GetComponent<Component::ComponentType::Material>())->GetSurfaceData();

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

	size_t pSizes[1] = { sizeof(data) };
	size_t pOffsets[1] = { 0 };
	RHIUpdateResourceDescriptor updateResourceDescriptor = {};
	{
		updateResourceDescriptor.bindingCount = 1;
		updateResourceDescriptor.pBindingResources = resource;
		updateResourceDescriptor.pBuffer = m_pUniformBuffer;
		updateResourceDescriptor.pSize = pSizes;
		updateResourceDescriptor.pOffsets = pOffsets;
		updateResourceDescriptor.pGroup = m_pGroup;
	}
	context->UpdateResourceToGroup(&updateResourceDescriptor);

	RHITextureViewDescriptor depthViewDescriptor = {};
	{
		depthViewDescriptor.format = Format::D32_SFloat;
		depthViewDescriptor.mipCount = 1;
		depthViewDescriptor.baseMipLevel = 0;
		depthViewDescriptor.arrayLayerCount = 1;
		depthViewDescriptor.baseArrayLayer = 0;
		depthViewDescriptor.dimension = Dimension::Texture2D;
		depthViewDescriptor.imageAspect = IMAGE_ASPECT_DEPTH;
	}
	std::vector<RHITextureView*> depthViews;
	m_pRenderTargets.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		std::vector<RHITextureView*> textureViews = { context->GetTextureView(i), m_pDepthTextures[i]->CreateTextureView(&depthViewDescriptor) };
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

	m_pCommandBuffers = context->GetCommandBuffer(ScriptableRenderPipeline::g_maxFrame, false);
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

	RHICommandBuffer *cmd = m_pCommandBuffers[ScriptableRenderPipeline::g_currentFrame];

	SurfaceData surfaceData = static_cast<Material*>(GameObject::Find("Cube")->GetComponent<Component::ComponentType::Material>())->GetSurfaceData();
	UniformData data =
	{
		cameraData->MatrixVP,
		glm::vec4(-2.0f, 2.0f, -2.0f, 1.0f),
		glm::vec4(cameraData->Position, 1.0f),
		glm::vec4(surfaceData.albedo, surfaceData.roughness)
	};
	m_pUniformBuffer->LoadData(&data, sizeof(data));

	size_t pSizes[1] = { sizeof(data) };
	size_t pOffsets[1] = { 0 };
	BindingResource resource[1] =
	{
		{0, ResourceType::UniformBuffer, 1, ShaderStage::vertex},
	};
	RHIUpdateResourceDescriptor updateResourceDescriptor = {};
	{
		updateResourceDescriptor.bindingCount = 1;
		updateResourceDescriptor.pBindingResources = resource;
		updateResourceDescriptor.pBuffer = m_pUniformBuffer;
		updateResourceDescriptor.pSize = pSizes;
		updateResourceDescriptor.pOffsets = pOffsets;
		updateResourceDescriptor.pGroup = m_pGroup;
	}
	context->UpdateResourceToGroup(&updateResourceDescriptor);

	cmd->BeginScopePass("Test");
	{
		RHIGraphicsEncoder* encoder = cmd->GetGraphicsEncoder();
		RHIRenderPassBeginDescriptor renderPassBeginDescriptor = {};
		{
			renderPassBeginDescriptor.renderPass = m_pRenderPass;
			renderPassBeginDescriptor.renderTarget = m_pRenderTargets[ScriptableRenderPipeline::g_currentImage];
		}
		encoder->BeginPass(&renderPassBeginDescriptor);
		TextureBarrier textureBarriers[] =
		{
			//{ context->GetTexture(ScriptableRenderPipeline::g_currentImage), AttachmentLayout::Undefined, AttachmentLayout::ColorBuffer, 0, ACCESS_COLOR_ATTACHMENT_READ | ACCESS_COLOR_ATTACHMENT_WRITE },
			{ m_pDepthTextures[ScriptableRenderPipeline::g_currentImage], AttachmentLayout::Undefined, AttachmentLayout::DepthBuffer, 0, ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_COLOR_ATTACHMENT_WRITE }
		};
		RHIBarrierDescriptor barrierDescriptor = {};
		{
			barrierDescriptor.textureCount = 1;
			barrierDescriptor.pTextureBarriers = textureBarriers;
			barrierDescriptor.srcStage = PIPELINE_STAGE_TOP_OF_PIPE;
			barrierDescriptor.dstStage = PIPELINE_STAGE_EARLY_FRAGMENT_TESTS;
		}
		encoder->ResourceBarrier(&barrierDescriptor);
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
