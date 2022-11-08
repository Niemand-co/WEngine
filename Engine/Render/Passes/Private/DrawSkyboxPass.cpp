#include "pch.h"
#include "Render/Passes/Public/DrawSkyboxPass.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "RHI/Public/RHIHeads.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Scene/Components/Public/Camera.h"
#include "Utils/Public/Window.h"
#include "Editor/Public/Screen.h"

glm::vec4 DrawSkyboxPass::topColor = glm::vec4(1.0f);
glm::vec4 DrawSkyboxPass::bottomColor = glm::vec4(1.0f);

DrawSkyboxPass::DrawSkyboxPass()
{
}

DrawSkyboxPass::~DrawSkyboxPass()
{
}

struct UniformData
{
	glm::mat4 V;
	glm::mat4 P;
	glm::vec4 cameraPos;
	glm::vec4 topColor;
	glm::vec4 bottomColor;
};

void DrawSkyboxPass::Setup(RHIContext* context, CameraData* cameraData)
{
	ShaderCodeBlob* vertBlob = new ShaderCodeBlob("../assets/SkyboxShaderVert.spv");
	RHIShaderDescriptor vertShaderDescriptor = {};
	{
		vertShaderDescriptor.entryName = "vert";
		vertShaderDescriptor.shaderStage = SHADER_STAGE_VERTEX;
		vertShaderDescriptor.pCode = vertBlob->GetCode();
		vertShaderDescriptor.codeSize = vertBlob->GetSize();
	}
	RHIShader* vertShader = m_pDevice->CreateShader(&vertShaderDescriptor);

	ShaderCodeBlob* fragBlob = new ShaderCodeBlob("../assets/SkyboxShaderFrag.spv");
	RHIShaderDescriptor fragShaderDescriptor = {};
	{
		fragShaderDescriptor.entryName = "frag";
		fragShaderDescriptor.shaderStage = SHADER_STAGE_FRAGMENT;
		fragShaderDescriptor.pCode = fragBlob->GetCode();
		fragShaderDescriptor.codeSize = fragBlob->GetSize();
	}
	RHIShader* fragShader = m_pDevice->CreateShader(&fragShaderDescriptor);

	RHIAttachmentDescriptor attachmentDescriptors[] = 
	{
		{ Format::A16R16G16B16_SFloat, 1, AttachmentLoadOP::Load, AttachmentStoreOP::Store, AttachmentLoadOP::DontCare, AttachmentStoreOP::DontCare, AttachmentLayout::ColorBuffer, AttachmentLayout::ColorBuffer },
		{ Format::D16_Unorm, 1, AttachmentLoadOP::DontCare, AttachmentStoreOP::DontCare, AttachmentLoadOP::Load, AttachmentStoreOP::Store, AttachmentLayout::DepthBuffer, AttachmentLayout::DepthBuffer }
	};

	SubPassAttachment colorAttachments[] = { { 0, AttachmentLayout::ColorBuffer } };
	SubPassAttachment depthStencilAttachment = { 1, AttachmentLayout::DepthBuffer };

	RHISubPassDescriptor subpassDescriptor = {};
	{
		subpassDescriptor.colorAttachmentCount = 1;
		subpassDescriptor.pColorAttachments = colorAttachments;
		subpassDescriptor.pDepthStencilAttachment = &depthStencilAttachment;
		subpassDescriptor.dependedPass = -1;
		subpassDescriptor.dependedStage = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		subpassDescriptor.waitingStage = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT | PIPELINE_STAGE_FRAGMENT_SHADER;
		subpassDescriptor.waitingAccess = ACCESS_COLOR_ATTACHMENT_READ | ACCESS_COLOR_ATTACHMENT_WRITE;
	}

	RHIRenderPassDescriptor renderpassDescriptor = {};
	{
		renderpassDescriptor.attachmentCount = 2;
		renderpassDescriptor.pAttachmentDescriptors = attachmentDescriptors;
		renderpassDescriptor.subpassCount = 1;
		renderpassDescriptor.pSubPassDescriptors = &subpassDescriptor;
	}
	m_pRenderPass = m_pDevice->CreateRenderPass(&renderpassDescriptor);

	RHIDepthStencilDescriptor depthStencilDescriptor = {};
	{
		depthStencilDescriptor.depthWriteEnabled = true;
		depthStencilDescriptor.depthTestEnabled = true;
		depthStencilDescriptor.depthBoundsTest = false;
		depthStencilDescriptor.depthCompareOP = DepthCompareOP::LE;
		depthStencilDescriptor.minDepth = 0.0f;
		depthStencilDescriptor.maxDepth = 1.0f;
	}

	RHIBlendDescriptor blendDescriptor = {};
	{
		blendDescriptor.blendEnabled = false;
		blendDescriptor.colorBlendOP = BlendOP::BlendAdd;
		blendDescriptor.colorSrcFactor = BlendFactor::FactorSrcAlpha;
		blendDescriptor.colorDstFactor = BlendFactor::FactorOneMinusSrcAlpha;
		blendDescriptor.alphaBlendOP = BlendOP::BlendAdd;
		blendDescriptor.alphaSrcFactor = BlendFactor::FactorOne;
		blendDescriptor.alphaDstFactor = BlendFactor::FactorZero;
	}

	m_pMesh = Mesh::GetCube();
	RHIVertexInputDescriptor vertexInputDescriptor = m_pMesh->GetVertexInputDescriptor();

	BindingResource resource[] = 
	{
		{ 0, ResourceType::UniformBuffer, 1, SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT },
		{ 1, ResourceType::Sampler, 1, SHADER_STAGE_FRAGMENT },
		{ 2, ResourceType::Texture, 6, SHADER_STAGE_FRAGMENT},
	};
	RHIGroupLayoutDescriptor groupLayoutDescriptor = {};
	{
		groupLayoutDescriptor.bindingCount = 3;
		groupLayoutDescriptor.pBindingResources = resource;
	}
	RHIGroupLayout *pGroupLayout = context->CreateGroupLayout(&groupLayoutDescriptor);

	RHIGroupDescriptor groupDescriptor = {};
	{
		groupDescriptor.pGroupLayout = pGroupLayout;
	}
	m_pGroup = context->CreateResourceGroup(&groupDescriptor);

	RHIPipelineResourceLayoutDescriptor pipelineResourceLayoutDescriptor = {};
	{
		pipelineResourceLayoutDescriptor.groupLayoutCount = 1;
		pipelineResourceLayoutDescriptor.pGroupLayout = pGroupLayout;
	}
	m_pPipelineLayout = context->CreatePipelineResourceLayout(&pipelineResourceLayoutDescriptor);

	RHIShader *shaders[] = { vertShader, fragShader };
	RHIPipelineStateObjectDescriptor psoDescriptor = {};
	{
		psoDescriptor.shaderCount = 2;
		psoDescriptor.pShader = shaders;
		psoDescriptor.renderPass = m_pRenderPass;
		psoDescriptor.blendDescriptor = &blendDescriptor;
		psoDescriptor.depthStencilDescriptor = &depthStencilDescriptor;
		psoDescriptor.vertexDescriptor = &vertexInputDescriptor;
		psoDescriptor.pipelineResourceLayout = m_pPipelineLayout;
	}
	m_pPSO = context->CreatePSO(&psoDescriptor);

	RHIBufferDescriptor vertexBufferDescriptor = {};
	{
		vertexBufferDescriptor.size = m_pMesh->m_vertexCount * sizeof(Vertex);
		vertexBufferDescriptor.pData = m_pMesh->m_pVertices;
		vertexBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pVertexBuffer = context->CreateVertexBuffer(&vertexBufferDescriptor);

	RHIBufferDescriptor indexBufferDescriptor = {};
	{
		indexBufferDescriptor.size = m_pMesh->m_indexCount * sizeof(unsigned int);
		indexBufferDescriptor.pData = m_pMesh->m_pIndices;
		indexBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pIndexBuffer = context->CreateIndexBuffer(&indexBufferDescriptor);

	UniformData data = 
	{
		cameraData->MatrixV,
		cameraData->MatrixP,
		glm::vec4(cameraData->Position, 1.0f),
		topColor,
		bottomColor
	};
	RHIBufferDescriptor uniformBufferDescriptor = {};
	{
		uniformBufferDescriptor.size = sizeof(UniformData);
		uniformBufferDescriptor.pData = &data;
		uniformBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pUniformBuffer = context->CreateUniformBuffer(&uniformBufferDescriptor);
	
	BufferResourceInfo bufferInfo[] = 
	{
		{ m_pUniformBuffer, 0, sizeof(UniformData) },
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

	RHITextureDescriptor textureDescriptor = {};
	{
		textureDescriptor.format = Format::A8R8G8B8_SNorm;
		textureDescriptor.width = 256;
		textureDescriptor.height = 256;
		textureDescriptor.layout = AttachmentLayout::Undefined;
		textureDescriptor.mipCount = 1;
		textureDescriptor.usage = IMAGE_USAGE_TRANSFER_DST | IMAGE_USAGE_SAMPLED;
	}
	m_pCubemap.resize(6);
	for(unsigned int i = 0; i < 6; ++i)
		m_pCubemap[i] = m_pDevice->CreateTexture(&textureDescriptor);

	m_pCubemap[0]->LoadData("assets/px.png", context);
	m_pCubemap[1]->LoadData("assets/nx.png", context);
	m_pCubemap[2]->LoadData("assets/py.png", context);
	m_pCubemap[3]->LoadData("assets/ny.png", context);
	m_pCubemap[4]->LoadData("assets/pz.png", context);
	m_pCubemap[5]->LoadData("assets/nz.png", context);

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

	RHITextureView* views[6];
	for (int i = 0; i < 6; ++i)
	{
		views[i] = m_pCubemap[i]->CreateTextureView(&uvd);
	}

	RHISamplerDescriptor samplerDescriptor = {};
	{
		samplerDescriptor.minFilter = Filter::Cube;
		samplerDescriptor.magFilter = Filter::Cube;
	}
	TextureResourceInfo samplerInfo[] = 
	{
		{ nullptr, m_pDevice->CreateSampler(&samplerDescriptor), AttachmentLayout::ReadOnlyColor },
	};
	RHIUpdateResourceDescriptor textureResourceDescriptor = {};
	{
		textureResourceDescriptor.bindingCount = 1;
		textureResourceDescriptor.pBindingResources = resource + 1;
		textureResourceDescriptor.pGroup = m_pGroup;
		textureResourceDescriptor.textureResourceCount = 1;
		textureResourceDescriptor.pTextureInfo = samplerInfo;
	}
	context->UpdateTextureResourceToGroup(&textureResourceDescriptor);

	TextureResourceInfo texturesInfo[] = 
	{
		{ views[0], nullptr, AttachmentLayout::ReadOnlyColor },
		{ views[1], nullptr, AttachmentLayout::ReadOnlyColor },
		{ views[2], nullptr, AttachmentLayout::ReadOnlyColor },
		{ views[3], nullptr, AttachmentLayout::ReadOnlyColor },
		{ views[4], nullptr, AttachmentLayout::ReadOnlyColor },
		{ views[5], nullptr, AttachmentLayout::ReadOnlyColor },
	};
	//RHIUpdateResourceDescriptor textureResourceDescriptor = {};
	{
		textureResourceDescriptor.bindingCount = 1;
		textureResourceDescriptor.pBindingResources = resource + 2;
		//textureResourceDescriptor.pGroup = m_pGroup;
		textureResourceDescriptor.textureResourceCount = 6;
		textureResourceDescriptor.pTextureInfo = texturesInfo;
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
			renderTargetDescriptor.width = WEngine::Screen::GetWidth();
			renderTargetDescriptor.height = WEngine::Screen::GetHeight();
		}
		m_pRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
	}

	m_pCommandBuffers = context->GetCommandBuffer(RHIContext::g_maxFrames, false);
}

void DrawSkyboxPass::Execute(RHIContext* context, CameraData* cameraData)
{
	RHICommandBuffer *cmd = m_pCommandBuffers[RHIContext::g_currentFrame];

	UniformData data =
	{
		cameraData->MatrixV,
		cameraData->MatrixP,
		glm::vec4(cameraData->Position, 1.0f),
		topColor,
		bottomColor
	};
	m_pUniformBuffer->LoadData(&data, sizeof(data));
	BindingResource resource[1] =
	{
		{0, ResourceType::UniformBuffer, 1, SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT},
	};
	BufferResourceInfo bufferInfo[] = 
	{
		{ m_pUniformBuffer, 0, sizeof(UniformData) },
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

	cmd->BeginScopePass("Skybox");
	{
		RHIGraphicsEncoder *encoder = cmd->GetGraphicsEncoder();

		RHIRenderPassBeginDescriptor renderpassBeginDescriptor = {};
		{
			renderpassBeginDescriptor.renderPass = m_pRenderPass;
			renderpassBeginDescriptor.renderTarget = m_pRenderTargets[RHIContext::g_currentFrame];
		}
		encoder->BeginPass(&renderpassBeginDescriptor);
		encoder->SetPipeline(m_pPSO);
		encoder->SetViewport(nullptr);
		encoder->SetScissor(nullptr);
		encoder->BindVertexBuffer(m_pVertexBuffer);
		encoder->BindIndexBuffer(m_pIndexBuffer);
		encoder->BindGroups(1, m_pGroup, m_pPipelineLayout);
		encoder->DrawIndexed(m_pMesh->m_indexCount, 0);
		encoder->EndPass();
		encoder->~RHIGraphicsEncoder();
		WEngine::Allocator::Get()->Deallocate(encoder);
	}
	cmd->EndScopePass();
	context->ExecuteCommandBuffer(cmd);
}
