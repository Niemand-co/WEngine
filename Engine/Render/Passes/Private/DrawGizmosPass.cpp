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
#include "Editor/Public/Debug.h"
#include "Scene/Public/GameObject.h"

DrawGizmosPass::DrawGizmosPass(ScriptableRenderer* pRenderer)
	: ScriptableRenderPass(pRenderer)
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
	};

	RHISubPassDependencyDescriptor dependencyDescriptor[] =
	{
		{ -1, PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT, ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE, 0, PIPELINE_STAGE_EARLY_FRAGMENT_TESTS | PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT, ACCESS_COLOR_ATTACHMENT_READ | ACCESS_COLOR_ATTACHMENT_WRITE | ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE },
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
	}

	RHISubPassDependencyDescriptor stencilDependencyDescriptor[] =
	{
		{ -1, PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT, 0, 0, PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT, ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE },
	};

	RHIRenderPassDescriptor stencilRenderPassDescriptor = {};
	{
		stencilRenderPassDescriptor.attachmentCount = 1;
		stencilRenderPassDescriptor.pAttachmentDescriptors = stencilAttachmentDescriptor;
		stencilRenderPassDescriptor.subpassCount = 1;
		stencilRenderPassDescriptor.pSubPassDescriptors = &stencilSubpassDescriptor;
		stencilRenderPassDescriptor.dependencyCount = 1;
		stencilRenderPassDescriptor.pDependencyDescriptors = stencilDependencyDescriptor;
	}
	m_pStencilRenderPass = RHIContext::GetDevice()->CreateRenderPass(&stencilRenderPassDescriptor);

}

DrawGizmosPass::~DrawGizmosPass()
{
}

struct SceneData
{
	glm::mat4 VP;
};

struct ObjectData
{
	glm::mat4 M;
	glm::vec4 color;
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
		depthStencilDescriptor.depthWriteEnabled = true;
		depthStencilDescriptor.depthCompareOP = CompareOP::Less;
		depthStencilDescriptor.minDepth = 0.0f;
		depthStencilDescriptor.maxDepth = 1.0f;
	}

	BindingResource resource[] = 
	{
		{ 0, ResourceType::UniformBuffer, 1, SHADER_STAGE_VERTEX },
		{ 1, ResourceType::DynamicUniformBuffer, 1, SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT },
	};

	RHIGroupLayoutDescriptor layoutDescriptor = {};
	{
		layoutDescriptor.bindingCount = 2;
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
		depthOnlyVertShaderDescriptor.entryName = "vert";
		depthOnlyVertShaderDescriptor.shaderStage = SHADER_STAGE_VERTEX;
		depthOnlyVertShaderDescriptor.pCode = dephOnlyVertBlob->GetCode();
		depthOnlyVertShaderDescriptor.codeSize = dephOnlyVertBlob->GetSize();
	}
	RHIShader* depthOnlyVertShader = m_pDevice->CreateShader(&depthOnlyVertShaderDescriptor);

	ShaderCodeBlob* depthOnlyFragBlob = new ShaderCodeBlob("../assets/DepthOnlyFrag.spv");
	RHIShaderDescriptor depthOnlyFragShaderDescriptor = {};
	{
		depthOnlyFragShaderDescriptor.entryName = "frag";
		depthOnlyFragShaderDescriptor.shaderStage = SHADER_STAGE_FRAGMENT;
		depthOnlyFragShaderDescriptor.pCode = depthOnlyFragBlob->GetCode();
		depthOnlyFragShaderDescriptor.codeSize = depthOnlyFragBlob->GetSize();
	}
	RHIShader* depthOnlyFragShader = m_pDevice->CreateShader(&depthOnlyFragShaderDescriptor);
	RHIShader *depthOnlyShaders[] = { depthOnlyVertShader, depthOnlyFragShader };

	{
		psoDescriptor.rasterizationStateDescriptor->polygonMode = PolygonMode::Triangle;
		psoDescriptor.depthStencilDescriptor->depthTestEnabled = true;
		psoDescriptor.depthStencilDescriptor->depthCompareOP = CompareOP::Always;
		psoDescriptor.depthStencilDescriptor->depthWriteEnabled = false;
		psoDescriptor.depthStencilDescriptor->stencilTestEnabled = false;
	}
	m_pDebugPSO = context->CreatePSO(&psoDescriptor);

	ShaderCodeBlob* debugDrawVertBlob = new ShaderCodeBlob("../assets/DebugDrawVert.spv");
	RHIShaderDescriptor debugDrawVertShaderDescriptor = {};
	{
		debugDrawVertShaderDescriptor.entryName = "vert";
		debugDrawVertShaderDescriptor.shaderStage = SHADER_STAGE_VERTEX;
		debugDrawVertShaderDescriptor.pCode = debugDrawVertBlob->GetCode();
		debugDrawVertShaderDescriptor.codeSize = debugDrawVertBlob->GetSize();
	}
	RHIShader* debugDrawVertShader = m_pDevice->CreateShader(&debugDrawVertShaderDescriptor);

	ShaderCodeBlob* debugDrawFragBlob = new ShaderCodeBlob("../assets/DebugDrawFrag.spv");
	RHIShaderDescriptor debugDrawFragShaderDescriptor = {};
	{
		debugDrawFragShaderDescriptor.entryName = "frag";
		debugDrawFragShaderDescriptor.shaderStage = SHADER_STAGE_FRAGMENT;
		debugDrawFragShaderDescriptor.pCode = debugDrawFragBlob->GetCode();
		debugDrawFragShaderDescriptor.codeSize = debugDrawFragBlob->GetSize();
	}
	RHIShader* debugDrawFragShader = m_pDevice->CreateShader(&debugDrawFragShaderDescriptor);
	RHIShader* debugDrawShaders[] = { debugDrawVertShader, debugDrawFragShader };

	{
		psoDescriptor.pShader = debugDrawShaders;
		psoDescriptor.rasterizationStateDescriptor->primitivePology = PrimitiveTopology::LineList;
		psoDescriptor.rasterizationStateDescriptor->polygonMode = PolygonMode::Line;
	}
	m_pDebugLinePSO = context->CreatePSO(&psoDescriptor);

	{
		psoDescriptor.renderPass = m_pStencilRenderPass;
		psoDescriptor.pShader = depthOnlyShaders;
		psoDescriptor.rasterizationStateDescriptor->polygonMode = PolygonMode::Triangle;
		psoDescriptor.rasterizationStateDescriptor->primitivePology = PrimitiveTopology::TriangleList;
		psoDescriptor.depthStencilDescriptor->stencilCompareOP = CompareOP::Always;
		psoDescriptor.depthStencilDescriptor->passOP = StencilFailedOP::Replace;
		psoDescriptor.depthStencilDescriptor->stencilFailedOP = StencilFailedOP::Replace;
		psoDescriptor.depthStencilDescriptor->depthFailedOP = StencilFailedOP::Replace;
		psoDescriptor.depthStencilDescriptor->depthWriteEnabled = true;
		psoDescriptor.depthStencilDescriptor->depthCompareOP = CompareOP::Less;
		psoDescriptor.depthStencilDescriptor->stencilTestEnabled = true;
	}
	m_pStencilPSO = context->CreatePSO(&psoDescriptor);

	RHIGroupDescriptor groupDescriptor = {};
	{
		groupDescriptor.pGroupLayout = layout;
	}
	m_pGroup = context->CreateResourceGroup(&groupDescriptor);

	RHIBufferDescriptor bufferDescriptor = {};
	{
		bufferDescriptor.dataSize = sizeof(SceneData);
		bufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pSceneBuffer = context->CreateUniformBuffer(&bufferDescriptor);

	{
		bufferDescriptor.dataSize = sizeof(ObjectData);
		bufferDescriptor.count = 10;
		bufferDescriptor.isDynamic = true;
		bufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE;
	}
	m_pObjectBuffer = context->CreateUniformBuffer(&bufferDescriptor);

	m_pObjectBuffer->Resize(4);
	m_pObjectBuffer->SetDataSize(sizeof(ObjectData));
	m_pSceneBuffer->SetDataSize(sizeof(SceneData));
	RHIBindingDescriptor bindingDescriptors[] = 
	{
		{ m_pSceneBuffer->Size(), m_pSceneBuffer->GetBufferInfo() },
		{ m_pObjectBuffer->Size(), m_pObjectBuffer->GetBufferInfo() },
	};
	RHIUpdateResourceDescriptor updateDescriptor = {};
	{
		updateDescriptor.bindingCount = 2;
		updateDescriptor.pBindingResources = resource;
		updateDescriptor.pBindingDescriptors = bindingDescriptors;
		updateDescriptor.pGroup = m_pGroup[0];
	}
	context->UpdateUniformResourceToGroup(&updateDescriptor);

	m_pStencils.Resize(RHIContext::g_maxFrames);
	m_pStencilViews.Resize(RHIContext::g_maxFrames);
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

	m_pRenderTargets.Resize(3);
	for (int i = 0; i < 3; ++i)
	{
		CameraRenderTarget& target = cameraData->camera->GetRenderTarget(i);
		WEngine::WArray<RHITextureView*> textureViews = { target.pColorTexture, m_pStencilViews[i] };
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

	m_pStencilRenderTargets.Resize(3);
	for (int i = 0; i < 3; ++i)
	{
		WEngine::WArray<RHITextureView*> textureViews = { m_pStencilViews[i] };
		RHIRenderTargetDescriptor renderTargetDescriptor = {};
		{
			renderTargetDescriptor.bufferCount = 1;
			renderTargetDescriptor.pBufferView = textureViews.GetData();
			renderTargetDescriptor.renderPass = m_pStencilRenderPass;
			renderTargetDescriptor.width = WEngine::Screen::GetWidth();
			renderTargetDescriptor.height = WEngine::Screen::GetHeight();
		}
		m_pStencilRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
	}

	m_pCommandBuffers = context->GetCommandBuffer(RHIContext::g_maxFrames, false);

	m_pAxisMesh = Mesh::GetArrow();
}

void DrawGizmosPass::Execute(RHIContext* context, CameraData* cameraData)
{
	if(WEngine::Editor::GetSelectedObjectCount() == 0)
		return;

	WEngine::WArray<GameObject*>& selectedObjects = WEngine::Editor::GetSelectedObject();

	RHICommandBuffer* cmd = m_pCommandBuffers[RHIContext::g_currentFrame];

	cmd->BeginScopePass("Gizmos", m_pRenderPass, 0, m_pRenderTargets[RHIContext::g_currentImage]);
	{
		RHIGraphicsEncoder* encoder = cmd->GetGraphicsEncoder();

		Transformer* pTransformer = selectedObjects[0]->GetComponent<Transformer>();
		glm::mat4 objectMatrix = pTransformer->GetTranslateMatrix();
		ObjectData arrowData[3] =
		{
			{ objectMatrix * glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ objectMatrix, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ objectMatrix * glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) }
		};
		for (unsigned int i = 0; i < 3; ++i)
		{
			m_pObjectBuffer->LoadData(arrowData + i, sizeof(ObjectData), i + 1);
		}
		SceneData sceneData = { cameraData->MatrixVP };
		m_pSceneBuffer->LoadData(&sceneData, sizeof(sceneData));
		
		ClearValue values[]{ {glm::vec4(1.f, 1.f, 1.f, 1.f), true }, { glm::vec4(1.f, 0.f, 0.f, 0.f), false } };
		RHIRenderPassBeginDescriptor renderpassBeginDescriptor = {};
		{
			renderpassBeginDescriptor.renderPass = m_pStencilRenderPass;
			renderpassBeginDescriptor.renderTarget = m_pStencilRenderTargets[RHIContext::g_currentFrame];
			renderpassBeginDescriptor.clearCount = 1;
			renderpassBeginDescriptor.pClearValues = values + 1;
		}
		encoder->BeginPass(&renderpassBeginDescriptor);
		encoder->SetPipeline(m_pStencilPSO);
		encoder->SetViewport({ (float)WEngine::Screen::GetWidth(), (float)WEngine::Screen::GetHeight(), 0, 0 });
		encoder->SetScissor({ WEngine::Screen::GetWidth(), WEngine::Screen::GetHeight(), 0, 0 });
		for (unsigned int i = 0; i < selectedObjects.Size(); ++i)
		{
			MeshFilter* filter = selectedObjects[i]->GetComponent<MeshFilter>();
			if (filter == nullptr)
				continue;

			Mesh* pMesh = filter->GetStaticMesh();
			ObjectData objectData = { selectedObjects[i]->GetComponent<Transformer>()->GetLocalToWorldMatrix(), glm::vec4(1.0f, 0.55f, 0.0f, 1.0f) };
			m_pObjectBuffer->LoadData(&objectData, sizeof(objectData), 0 );
			m_pObjectBuffer->Flush(4);

			unsigned int offset = 0;
			encoder->BindVertexBuffer(pMesh->GetVertexBuffer());
			encoder->BindIndexBuffer(pMesh->GetIndexBuffer());
			encoder->BindGroups(1, m_pGroup[0], m_pResourceLayout, 1, &offset);
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
		for (unsigned int i = 0; i < selectedObjects.Size(); ++i)
		{
			MeshFilter *filter = selectedObjects[i]->GetComponent<MeshFilter>();
			if(filter == nullptr)
				continue;

			Mesh *pMesh = filter->GetStaticMesh();

			unsigned int offset = 0;
			encoder->BindVertexBuffer(pMesh->GetVertexBuffer());
			encoder->BindIndexBuffer(pMesh->GetIndexBuffer());
			encoder->BindGroups(1, m_pGroup[0], m_pResourceLayout, 1, &offset);
			encoder->SetLineWidth(10.0f);
			encoder->DrawIndexed(pMesh->m_indexCount, 0);
		}
		
		encoder->SetPipeline(m_pDebugPSO);
		encoder->BindVertexBuffer(m_pAxisMesh->GetVertexBuffer());
		encoder->BindIndexBuffer(m_pAxisMesh->GetIndexBuffer());
		for (unsigned int i = 0; i < 3; ++i)
		{
			unsigned int offset = (i + 1) * m_pObjectBuffer->Alignment();
			encoder->BindGroups(1, m_pGroup[0], m_pResourceLayout, 1, &offset);
			encoder->SetLineWidth(1.0f);
			encoder->DrawIndexed(m_pAxisMesh->m_indexCount, 0);
		}

		encoder->SetPipeline(m_pDebugLinePSO);
		WEngine::Debug::Update();
		encoder->BindVertexBuffer(WEngine::Debug::GetLineVertexBuffer());
		encoder->BindIndexBuffer(WEngine::Debug::GetLineIndexBuffer());
		encoder->SetLineWidth(5.0f);
		encoder->DrawIndexed(WEngine::Debug::GetIndexCount(), 0);

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
		WEngine::WArray<RHITextureView*> textureViews = { cameraData->camera->GetRenderTarget(i).pColorTexture, m_pStencilViews[i] };
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

	m_pStencilRenderTargets.Resize(3);
	for (int i = 0; i < 3; ++i)
	{
		WEngine::WArray<RHITextureView*> textureViews = { m_pStencilViews[i] };
		RHIRenderTargetDescriptor renderTargetDescriptor = {};
		{
			renderTargetDescriptor.bufferCount = 1;
			renderTargetDescriptor.pBufferView = textureViews.GetData();
			renderTargetDescriptor.renderPass = m_pStencilRenderPass;
			renderTargetDescriptor.width = WEngine::Screen::GetWidth();
			renderTargetDescriptor.height = WEngine::Screen::GetHeight();
		}
		delete m_pStencilRenderTargets[i];
		m_pStencilRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
	}
}
