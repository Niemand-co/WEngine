#include "pch.h"
#include "Render/Passes/Public/DrawGizmosPass.h"
#include "RHI/Public/RHIHeads.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Scene/Components/Public/Camera.h"
#include "Editor/Public/Screen.h"
#include "Editor/Public/Editor.h"
#include "Scene/Public/GameObject.h"

DrawGizmosPass::DrawGizmosPass()
{
	RHIAttachmentDescriptor attachmentDescriptors[] = 
	{
		{ Format::A16R16G16B16_SFloat, 1, AttachmentLoadOP::Load, AttachmentStoreOP::Store, AttachmentLoadOP::Load, AttachmentStoreOP::Store, AttachmentLayout::ColorBuffer, AttachmentLayout::Present },
		{ Format::D24_UNORM_S8_UINT, 1, AttachmentLoadOP::Load, AttachmentStoreOP::Store, AttachmentLoadOP::Load, AttachmentStoreOP::Store, AttachmentLayout::DepthBuffer, AttachmentLayout::DepthBuffer },
	};

	SubPassAttachment colorAttachments[] = { { 0, AttachmentLayout::ColorBuffer } };
	SubPassAttachment stencilAttachment = { 1, AttachmentLayout::DepthBuffer };

	RHISubPassDescriptor subpassDescriptor = {};
	{
		subpassDescriptor.colorAttachmentCount = 1;
		subpassDescriptor.pColorAttachments = colorAttachments;
		subpassDescriptor.pDepthStencilAttachment = &stencilAttachment;
		subpassDescriptor.dependedPass = -1;
		subpassDescriptor.dependedStage = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		subpassDescriptor.waitingAccess = ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE;
		subpassDescriptor.waitingStage = PIPELINE_STAGE_EARLY_FRAGMENT_TESTS | PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		subpassDescriptor.waitingAccess = ACCESS_COLOR_ATTACHMENT_READ | ACCESS_COLOR_ATTACHMENT_WRITE | ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE;
	};

	RHIRenderPassDescriptor renderPassDescriptor = {};
	{
		renderPassDescriptor.attachmentCount = 2;
		renderPassDescriptor.pAttachmentDescriptors = attachmentDescriptors;
		renderPassDescriptor.subpassCount = 1;
		renderPassDescriptor.pSubPassDescriptors = &subpassDescriptor;
	}
	m_pRenderPass = RHIContext::GetDevice()->CreateRenderPass(&renderPassDescriptor);

	RHIAttachmentDescriptor stencilAttachmentDescriptor[] = 
	{
		{ Format::D24_UNORM_S8_UINT, 1, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLayout::DepthBuffer, AttachmentLayout::DepthBuffer },
	};
	stencilAttachment.attachmentIndex = 0;

	RHISubPassDescriptor stencilSubpassDescriptor = {};
	{
		stencilSubpassDescriptor.colorAttachmentCount = 0;
		stencilSubpassDescriptor.pColorAttachments = nullptr;
		stencilSubpassDescriptor.pDepthStencilAttachment = &stencilAttachment;
		stencilSubpassDescriptor.dependedPass = -1;
		stencilSubpassDescriptor.dependedStage = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		stencilSubpassDescriptor.waitingStage = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		stencilSubpassDescriptor.waitingAccess = ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE;
	}

	RHIRenderPassDescriptor stencilRenderPassDescriptor = {};
	{
		stencilRenderPassDescriptor.attachmentCount = 1;
		stencilRenderPassDescriptor.pAttachmentDescriptors = stencilAttachmentDescriptor;
		stencilRenderPassDescriptor.subpassCount = 1;
		stencilRenderPassDescriptor.pSubPassDescriptors = &stencilSubpassDescriptor;
	}
	m_pStencilRenderPass = RHIContext::GetDevice()->CreateRenderPass(&stencilRenderPassDescriptor);

}

DrawGizmosPass::~DrawGizmosPass()
{
}

struct UniformData
{
	glm::mat4 M;
	glm::mat4 VP;
};

void DrawGizmosPass::Setup(RHIContext* context, CameraData* cameraData)
{
	ShaderCodeBlob* vertBlob = new ShaderCodeBlob("../assets/OutlineVert.spv");
	RHIShaderDescriptor vertShaderDescriptor = {};
	{
		vertShaderDescriptor.entryName = "vert";
		vertShaderDescriptor.shaderStage = SHADER_STAGE_VERTEX;
		vertShaderDescriptor.pCode = vertBlob->GetCode();
		vertShaderDescriptor.codeSize = vertBlob->GetSize();
	}
	RHIShader* vertShader = m_pDevice->CreateShader(&vertShaderDescriptor);

	ShaderCodeBlob* fragBlob = new ShaderCodeBlob("../assets/OutlineFrag.spv");
	RHIShaderDescriptor fragShaderDescriptor = {};
	{
		fragShaderDescriptor.entryName = "frag";
		fragShaderDescriptor.shaderStage = SHADER_STAGE_FRAGMENT;
		fragShaderDescriptor.pCode = fragBlob->GetCode();
		fragShaderDescriptor.codeSize = fragBlob->GetSize();
	}
	RHIShader* fragShader = m_pDevice->CreateShader(&fragShaderDescriptor);

	RHIShader *shaders[] = { vertShader, fragShader };

	RHIBlendDescriptor blendDescriptor = {};
	{
		blendDescriptor.blendEnabled = true;
		blendDescriptor.alphaBlendOP = BlendOP::BlendAdd;
		blendDescriptor.alphaDstFactor = BlendFactor::FactorZero;
		blendDescriptor.alphaSrcFactor = BlendFactor::FactorOne;
		blendDescriptor.colorBlendOP = BlendOP::BlendAdd;
		blendDescriptor.colorDstFactor = BlendFactor::FactorOneMinusSrcAlpha;
		blendDescriptor.colorSrcFactor = BlendFactor::FactorSrcAlpha;
	}

	RHIDepthStencilDescriptor depthStencilDescriptor = {};
	{
		depthStencilDescriptor.stencilTestEnabled = true;
		depthStencilDescriptor.stencilCompareOP = CompareOP::NotEqual;
		depthStencilDescriptor.depthFailedOP = StencilFailedOP::Keep;
		depthStencilDescriptor.passOP = StencilFailedOP::Replace;
		depthStencilDescriptor.stencilFailedOP = StencilFailedOP::Keep;
		depthStencilDescriptor.stencilRef = 1;
		depthStencilDescriptor.depthTestEnabled = true;
		depthStencilDescriptor.depthCompareOP = CompareOP::Greater;
		depthStencilDescriptor.minDepth = 1.0f;
		depthStencilDescriptor.maxDepth = 0.0f;
	}

	BindingResource resource[] = 
	{
		{ 0, ResourceType::UniformBuffer, 1, SHADER_STAGE_VERTEX },
	};

	RHIGroupLayoutDescriptor layoutDescriptor = {};
	{
		layoutDescriptor.bindingCount = 1;
		layoutDescriptor.pBindingResources = resource;
	}
	RHIGroupLayout *layout = context->CreateGroupLayout(&layoutDescriptor);

	RHIPipelineResourceLayoutDescriptor resourceLayoutDescriptor = {};
	{
		resourceLayoutDescriptor.groupLayoutCount = 1;
		resourceLayoutDescriptor.pGroupLayout = layout;
	}
	m_pResourceLayout = context->CreatePipelineResourceLayout(&resourceLayoutDescriptor);

	RHIRasterizationStateDescriptor rasterizationStateDescriptor = {};
	{
		rasterizationStateDescriptor.polygonMode = PolygonMode::Line;
		rasterizationStateDescriptor.lineWidth = 1.0f;
	}

	RHIVertexInputDescriptor vertexInputDescriptor = Vertex::GetVertexInputDescriptor();

	RHIPipelineStateObjectDescriptor psoDescriptor = {};
	{
		psoDescriptor.shaderCount = 2;
		psoDescriptor.pShader = shaders;
		psoDescriptor.renderPass = m_pRenderPass;
		psoDescriptor.subpass = 0;
		psoDescriptor.blendDescriptor = &blendDescriptor;
		psoDescriptor.depthStencilDescriptor = &depthStencilDescriptor;
		psoDescriptor.pipelineResourceLayout = m_pResourceLayout;
		psoDescriptor.vertexDescriptor = &vertexInputDescriptor;
		psoDescriptor.rasterizationStateDescriptor = &rasterizationStateDescriptor;
	}
	m_pPSO = context->CreatePSO(&psoDescriptor);

	ShaderCodeBlob* dephOnlyVertBlob = new ShaderCodeBlob("../assets/DepthOnlyVert.spv");
	RHIShaderDescriptor depthOnlyVertShaderDescriptor = {};
	{
		vertShaderDescriptor.entryName = "vert";
		vertShaderDescriptor.shaderStage = SHADER_STAGE_VERTEX;
		vertShaderDescriptor.pCode = dephOnlyVertBlob->GetCode();
		vertShaderDescriptor.codeSize = dephOnlyVertBlob->GetSize();
	}
	RHIShader* depthOnlyVertShader = m_pDevice->CreateShader(&vertShaderDescriptor);

	ShaderCodeBlob* depthOnlyFragBlob = new ShaderCodeBlob("../assets/DepthOnlyFrag.spv");
	RHIShaderDescriptor depthOnlyFragShaderDescriptor = {};
	{
		fragShaderDescriptor.entryName = "frag";
		fragShaderDescriptor.shaderStage = SHADER_STAGE_FRAGMENT;
		fragShaderDescriptor.pCode = depthOnlyFragBlob->GetCode();
		fragShaderDescriptor.codeSize = depthOnlyFragBlob->GetSize();
	}
	RHIShader* depthOnlyFragShader = m_pDevice->CreateShader(&fragShaderDescriptor);
	RHIShader *depthOnlyShaders[] = { depthOnlyVertShader, depthOnlyFragShader };

	{
		psoDescriptor.renderPass = m_pStencilRenderPass;
		psoDescriptor.pShader = depthOnlyShaders;
		psoDescriptor.subpass = 0;
		psoDescriptor.rasterizationStateDescriptor->polygonMode = PolygonMode::Triangle;
		psoDescriptor.depthStencilDescriptor->stencilCompareOP = CompareOP::Always;
		psoDescriptor.depthStencilDescriptor->passOP = StencilFailedOP::Replace;
		psoDescriptor.depthStencilDescriptor->stencilFailedOP = StencilFailedOP::Replace;
		psoDescriptor.depthStencilDescriptor->depthFailedOP = StencilFailedOP::Replace;
	}
	m_pStencilPSO = context->CreatePSO(&psoDescriptor);

	RHIGroupDescriptor groupDescriptor = {};
	{
		groupDescriptor.pGroupLayout = layout;
	}
	m_pGroup = context->CreateResourceGroup(&groupDescriptor);

	RHIBufferDescriptor bufferDescriptor = {};
	{
		bufferDescriptor.size = sizeof(UniformData);
		bufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pBuffer = context->CreateUniformBuffer(&bufferDescriptor);

	BindingResource bindings[] =
	{
		{ 0, ResourceType::UniformBuffer, 1, SHADER_STAGE_VERTEX },
	};
	BufferResourceInfo info = {};
	{
		info.pBuffer = m_pBuffer;
		info.range = sizeof(UniformData);
		info.offset = 0;
	}
	RHIUpdateResourceDescriptor updateDescriptor = {};
	{
		updateDescriptor.bindingCount = 1;
		updateDescriptor.pBindingResources = bindings;
		updateDescriptor.bufferResourceCount = 1;
		updateDescriptor.pBufferInfo = &info;
		updateDescriptor.pGroup = m_pGroup[0];
	}
	context->UpdateUniformResourceToGroup(&updateDescriptor);

	m_pStencils.resize(RHIContext::g_maxFrames);
	m_pStencilViews.resize(RHIContext::g_maxFrames);
	RHITextureDescriptor textureDescriptor = {};
	{
		textureDescriptor.format = Format::D24_UNORM_S8_UINT;
		textureDescriptor.width = WEngine::Screen::GetWidth();
		textureDescriptor.height = WEngine::Screen::GetHeight();
		textureDescriptor.layerCount = 1;
		textureDescriptor.mipCount = 1;
		textureDescriptor.layout = AttachmentLayout::Undefined;
		textureDescriptor.usage = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT;
	}

	RHITextureViewDescriptor viewDescriptor = {};
	{
		viewDescriptor.format = Format::D24_UNORM_S8_UINT;
		viewDescriptor.arrayLayerCount = 1;
		viewDescriptor.baseArrayLayer = 0;
		viewDescriptor.mipCount = 1;
		viewDescriptor.baseMipLevel = 0;
		viewDescriptor.dimension = Dimension::Texture2D;
		viewDescriptor.imageAspect = IMAGE_ASPECT_STENCIL;
	}
	for (unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
	{
		m_pStencils[i] = m_pDevice->CreateTexture(&textureDescriptor);
		TextureBarrier barrier = { m_pStencils[i], AttachmentLayout::Undefined, AttachmentLayout::DepthBuffer, 0, ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE | ACCESS_DEPTH_STENCIL_ATTACHMENT_READ, IMAGE_ASPECT_STENCIL };
		RHIBarrierDescriptor barrierDescriptor = {};
		{
			barrierDescriptor.pTextureBarriers = &barrier;
			barrierDescriptor.srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			barrierDescriptor.dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		context->ResourceBarrier(&barrierDescriptor);
		m_pStencilViews[i] = m_pStencils[i]->CreateTextureView(&viewDescriptor);
	}

	m_pRenderTargets.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		RenderTarget& target = cameraData->camera->GetRenderTarget(i);
		std::vector<RHITextureView*> textureViews = { target.pColorTexture, m_pStencilViews[i] };
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

	m_pStencilRenderTargets.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		std::vector<RHITextureView*> textureViews = { m_pStencilViews[i] };
		RHIRenderTargetDescriptor renderTargetDescriptor = {};
		{
			renderTargetDescriptor.bufferCount = 1;
			renderTargetDescriptor.pBufferView = textureViews.data();
			renderTargetDescriptor.renderPass = m_pStencilRenderPass;
			renderTargetDescriptor.width = WEngine::Screen::GetWidth();
			renderTargetDescriptor.height = WEngine::Screen::GetHeight();
		}
		m_pStencilRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
	}

	m_pCommandBuffers = context->GetCommandBuffer(RHIContext::g_maxFrames, false);
}

void DrawGizmosPass::Execute(RHIContext* context, CameraData* cameraData)
{
	if(WEngine::Editor::GetSelectedObjectCount() == 0)
		return;

	std::vector<GameObject*>& selectedObjects = WEngine::Editor::GetSelectedObject();

	RHICommandBuffer* cmd = m_pCommandBuffers[RHIContext::g_currentFrame];

	cmd->BeginScopePass("Gizmos", m_pRenderPass, 0, m_pRenderTargets[RHIContext::g_currentImage]);
	{
		RHIGraphicsEncoder* encoder = cmd->GetGraphicsEncoder();

		ClearValue values[]{ {glm::vec4(1.f, 1.f, 1.f, 1.f), 0.0f, 0 }, { glm::vec4(), 0.0f, 0 } };
		RHIRenderPassBeginDescriptor renderpassBeginDescriptor = {};
		{
			renderpassBeginDescriptor.renderPass = m_pStencilRenderPass;
			renderpassBeginDescriptor.renderTarget = m_pStencilRenderTargets[RHIContext::g_currentFrame];
			renderpassBeginDescriptor.clearCount = 1;
			renderpassBeginDescriptor.pClearValues = values;
		}
		encoder->BeginPass(&renderpassBeginDescriptor);
		encoder->SetPipeline(m_pStencilPSO);
		encoder->SetViewport({ (float)WEngine::Screen::GetWidth(), (float)WEngine::Screen::GetHeight(), 0, 0 });
		encoder->SetScissor({ WEngine::Screen::GetWidth(), WEngine::Screen::GetHeight(), 0, 0 });
		for (unsigned int i = 0; i < selectedObjects.size(); ++i)
		{
			MeshFilter* filter = selectedObjects[i]->GetComponent<MeshFilter>();
			if (filter == nullptr)
				continue;

			Mesh* pMesh = filter->GetStaticMesh();
			UniformData data = { selectedObjects[i]->GetComponent<Transformer>()->GetLocalToWorldMatrix(), cameraData->MatrixVP };
			m_pBuffer->LoadData(&data, sizeof(data));

			encoder->BindVertexBuffer(pMesh->GetVertexBuffer());
			encoder->BindIndexBuffer(pMesh->GetIndexBuffer());
			encoder->BindGroups(1, m_pGroup[0], m_pResourceLayout);
			encoder->DrawIndexed(pMesh->m_indexCount, 0);
		}
		encoder->EndPass();

		{
			renderpassBeginDescriptor.renderPass = m_pRenderPass;
			renderpassBeginDescriptor.renderTarget = m_pRenderTargets[RHIContext::g_currentFrame];
			renderpassBeginDescriptor.clearCount = 2;
		}
		encoder->BeginPass(&renderpassBeginDescriptor);
		encoder->SetPipeline(m_pPSO);
		encoder->SetViewport({ (float)WEngine::Screen::GetWidth(), (float)WEngine::Screen::GetHeight(), 0, 0 });
		encoder->SetScissor({ WEngine::Screen::GetWidth(), WEngine::Screen::GetHeight(), 0, 0 });
		for (unsigned int i = 0; i < selectedObjects.size(); ++i)
		{
			MeshFilter *filter = selectedObjects[i]->GetComponent<MeshFilter>();
			if(filter == nullptr)
				continue;

			Mesh *pMesh = filter->GetStaticMesh();
			UniformData data = { selectedObjects[i]->GetComponent<Transformer>()->GetLocalToWorldMatrix(), cameraData->MatrixVP };
			m_pBuffer->LoadData(&data, sizeof(data));

			encoder->BindVertexBuffer(pMesh->GetVertexBuffer());
			encoder->BindIndexBuffer(pMesh->GetIndexBuffer());
			encoder->BindGroups(1, m_pGroup[0], m_pResourceLayout);
			encoder->SetLineWidth(10.0f);
			encoder->DrawIndexed(pMesh->m_indexCount, 0);
		}
		encoder->EndPass();

		encoder->~RHIGraphicsEncoder();
		WEngine::Allocator::Get()->Deallocate(encoder);
	}
	cmd->EndScopePass();
	context->ExecuteCommandBuffer(cmd);
}

void DrawGizmosPass::UpdateRenderTarget(CameraData* cameraData)
{
	RHITextureDescriptor textureDescriptor = {};
	{
		textureDescriptor.format = Format::D24_UNORM_S8_UINT;
		textureDescriptor.width = WEngine::Screen::GetWidth();
		textureDescriptor.height = WEngine::Screen::GetHeight();
		textureDescriptor.layerCount = 1;
		textureDescriptor.mipCount = 1;
		textureDescriptor.layout = AttachmentLayout::Undefined;
		textureDescriptor.usage = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT;
	}

	RHITextureViewDescriptor viewDescriptor = {};
	{
		viewDescriptor.format = Format::D24_UNORM_S8_UINT;
		viewDescriptor.arrayLayerCount = 1;
		viewDescriptor.baseArrayLayer = 0;
		viewDescriptor.mipCount = 1;
		viewDescriptor.baseMipLevel = 0;
		viewDescriptor.dimension = Dimension::Texture2D;
		viewDescriptor.imageAspect = IMAGE_ASPECT_STENCIL;
	}

	for (int i = 0; i < 3; ++i)
	{
		delete m_pStencilViews[i];
		delete m_pStencils[i];
		m_pStencils[i] = RHIContext::GetDevice()->CreateTexture(&textureDescriptor);
		m_pStencilViews[i] = m_pStencils[i]->CreateTextureView(&viewDescriptor);
		std::vector<RHITextureView*> textureViews = { cameraData->camera->GetRenderTarget(i).pColorTexture, m_pStencilViews[i] };
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

	m_pStencilRenderTargets.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		std::vector<RHITextureView*> textureViews = { m_pStencilViews[i] };
		RHIRenderTargetDescriptor renderTargetDescriptor = {};
		{
			renderTargetDescriptor.bufferCount = 1;
			renderTargetDescriptor.pBufferView = textureViews.data();
			renderTargetDescriptor.renderPass = m_pStencilRenderPass;
			renderTargetDescriptor.width = WEngine::Screen::GetWidth();
			renderTargetDescriptor.height = WEngine::Screen::GetHeight();
		}
		delete m_pStencilRenderTargets[i];
		m_pStencilRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
	}
}
