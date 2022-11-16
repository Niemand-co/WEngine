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
	};

	SubPassAttachment colorAttachments[] = { { 0, AttachmentLayout::ColorBuffer } };

	RHISubPassDescriptor subpassDescriptor = {};
	{
		subpassDescriptor.colorAttachmentCount = 1;
		subpassDescriptor.pColorAttachments = colorAttachments;
		subpassDescriptor.dependedPass = -1;
		subpassDescriptor.dependedStage = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		subpassDescriptor.waitingStage = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		subpassDescriptor.waitingAccess = ACCESS_COLOR_ATTACHMENT_READ | ACCESS_COLOR_ATTACHMENT_WRITE;
	};

	RHIRenderPassDescriptor renderPassDescriptor = {};
	{
		renderPassDescriptor.attachmentCount = 1;
		renderPassDescriptor.pAttachmentDescriptors = attachmentDescriptors;
		renderPassDescriptor.subpassCount = 1;
		renderPassDescriptor.pSubPassDescriptors = &subpassDescriptor;
	}
	m_pRenderPass = RHIContext::GetDevice()->CreateRenderPass(&renderPassDescriptor);
}

DrawGizmosPass::~DrawGizmosPass()
{
}

struct UniformData
{
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
		depthStencilDescriptor.depthBoundsTest = false;
		depthStencilDescriptor.depthTestEnabled = false;
		depthStencilDescriptor.depthWriteEnabled = false;
		depthStencilDescriptor.depthCompareOP = DepthCompareOP::Greater;
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
		rasterizationStateDescriptor.lineWidth = 5.0f;
		rasterizationStateDescriptor.primitivePology = PrimitiveTopology::LineList;
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

	RHIGroupDescriptor groupDescriptor = {};
	{
		groupDescriptor.pGroupLayout = layout;
	}
	m_pGroup = context->CreateResourceGroup(&groupDescriptor);

	UniformData data = { cameraData->MatrixVP };
	RHIBufferDescriptor bufferDescriptor = {};
	{
		bufferDescriptor.pData = &data;
		bufferDescriptor.size = sizeof(data);
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
		info.range = sizeof(data);
		info.offset = 0;
	}
	RHIUpdateResourceDescriptor updateDescriptor = {};
	{
		updateDescriptor.bindingCount = 1;
		updateDescriptor.pBindingResources = bindings;
		updateDescriptor.bufferResourceCount = 1;
		updateDescriptor.pBufferInfo = &info;
		updateDescriptor.pGroup = m_pGroup;
	}
	context->UpdateUniformResourceToGroup(&updateDescriptor);

	m_pMesh = GameObject::Find("Cube")->GetComponent<MeshFilter>()->GetStaticMesh();

	RHIBufferDescriptor vertexBufferDescriptor = {};
	{
		vertexBufferDescriptor.pData = m_pMesh->m_pVertices;
		vertexBufferDescriptor.size = m_pMesh->m_vertexCount * sizeof(Vertex);
		vertexBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pVertexBuffer = context->CreateVertexBuffer(&vertexBufferDescriptor);

	RHIBufferDescriptor indexBufferDescriptor = {};
	{
		indexBufferDescriptor.pData = m_pMesh->m_pIndices;
		indexBufferDescriptor.size = m_pMesh->m_indexCount * sizeof(unsigned int);
		indexBufferDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pIndexBuffer = context->CreateIndexBuffer(&indexBufferDescriptor);

	RHIBufferDescriptor rayDescriptor = {};
	{
		rayDescriptor.pData = &WEngine::Editor::g_ray;
		rayDescriptor.size = sizeof(ray_line);
		rayDescriptor.memoryType = MEMORY_PROPERTY_HOST_VISIBLE | MEMORY_PROPERTY_HOST_COHERENT;
	}
	m_pRayBuffer = context->CreateVertexBuffer(&rayDescriptor);

	m_pRenderTargets.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		RenderTarget& target = cameraData->camera->GetRenderTarget(i);
		std::vector<RHITextureView*> textureViews = { target.pColorTexture };
		RHIRenderTargetDescriptor renderTargetDescriptor = {};
		{
			renderTargetDescriptor.bufferCount = 1;
			renderTargetDescriptor.pBufferView = textureViews.data();
			renderTargetDescriptor.renderPass = m_pRenderPass;
			renderTargetDescriptor.width = WEngine::Screen::GetWidth();
			renderTargetDescriptor.height = WEngine::Screen::GetHeight();
		}
		m_pRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
	}

	m_pCommandBuffers = context->GetCommandBuffer(RHIContext::g_maxFrames, false);
}

void DrawGizmosPass::Execute(RHIContext* context, CameraData* cameraData)
{
	//if(WEngine::Editor::GetSelectedObjectCount() == 0)
	//	return;

	UniformData data = { cameraData->MatrixVP };
	BindingResource bindings[] = 
	{
		{ 0, ResourceType::UniformBuffer, 1, SHADER_STAGE_VERTEX },
	};
	m_pBuffer->LoadData(&data, sizeof(data));
	BufferResourceInfo info = {};
	{
		info.pBuffer = m_pBuffer;
		info.range = sizeof(data);
		info.offset = 0;
	}
	RHIUpdateResourceDescriptor updateDescriptor = {};
	{
		updateDescriptor.bindingCount = 1;
		updateDescriptor.pBindingResources = bindings;
		updateDescriptor.bufferResourceCount = 1;
		updateDescriptor.pBufferInfo = &info;
		updateDescriptor.pGroup = m_pGroup;
	}
	context->UpdateUniformResourceToGroup(&updateDescriptor);

	m_pRayBuffer->LoadData(&WEngine::Editor::g_ray, sizeof(ray_line));

	RHICommandBuffer* cmd = m_pCommandBuffers[RHIContext::g_currentFrame];

	cmd->BeginScopePass("Skybox", m_pRenderPass, 0, m_pRenderTargets[RHIContext::g_currentImage]);
	{
		RHIGraphicsEncoder* encoder = cmd->GetGraphicsEncoder();

		RHIRenderPassBeginDescriptor renderpassBeginDescriptor = {};
		{
			renderpassBeginDescriptor.renderPass = m_pRenderPass;
			renderpassBeginDescriptor.renderTarget = m_pRenderTargets[RHIContext::g_currentFrame];
		}
		encoder->BeginPass(&renderpassBeginDescriptor);
		encoder->SetPipeline(m_pPSO);
		encoder->SetViewport({ (float)WEngine::Screen::GetWidth(), (float)WEngine::Screen::GetHeight(), 0, 0 });
		encoder->SetScissor({ WEngine::Screen::GetWidth(), WEngine::Screen::GetHeight(), 0, 0 });
		//encoder->BindVertexBuffer(m_pVertexBuffer);
		//encoder->BindIndexBuffer(m_pIndexBuffer);
		encoder->BindGroups(1, m_pGroup, m_pResourceLayout);
		//encoder->DrawIndexed(m_pMesh->m_indexCount, 0);
		encoder->BindVertexBuffer(m_pRayBuffer);
		encoder->DrawVertexArray();
		encoder->EndPass();
		encoder->~RHIGraphicsEncoder();
		WEngine::Allocator::Get()->Deallocate(encoder);
	}
	cmd->EndScopePass();
	context->ExecuteCommandBuffer(cmd);
}

void DrawGizmosPass::UpdateRenderTarget(CameraData* cameraData)
{
	for (int i = 0; i < 3; ++i)
	{
		std::vector<RHITextureView*> textureViews = { cameraData->camera->GetRenderTarget(i).pColorTexture };
		RHIRenderTargetDescriptor renderTargetDescriptor = {};
		{
			renderTargetDescriptor.bufferCount = 1;
			renderTargetDescriptor.pBufferView = textureViews.data();
			renderTargetDescriptor.renderPass = m_pRenderPass;
			renderTargetDescriptor.width = WEngine::Screen::GetWidth();
			renderTargetDescriptor.height = WEngine::Screen::GetHeight();
		}
		delete m_pRenderTargets[i];
		m_pRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
	}
}
