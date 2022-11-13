#include "pch.h"
#include "Scene/Components/Public/Camera.h"
#include "Scene/Public/GameObject.h"
#include "RHI/Public/RHITexture.h"
#include "RHI/Public/RHITextureView.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIDevice.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Render/Public/ScriptableRenderer.h"
#include "Scene/Public/World.h"
#include "Editor/Public/Screen.h"

Camera::Camera(GameObject* pGameObject, const float& fov, const float& aspect, const float& nearPlane, const float& farPlane)
	: m_fov(fov), m_aspect(aspect), m_nearPlane(nearPlane), m_farPlane(farPlane), Component(pGameObject)
{
	m_type = Component::ComponentType::Camera;
	World::GetWorld()->AddCamera(this);
	UpdateProjectionMatrix();
	m_rendertargets.resize(RHIContext::g_maxFrames);
	m_textureResources.resize(RHIContext::g_maxFrames * 2u);

	for(unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
	{
		RHITextureDescriptor descriptor = {};
		{
			descriptor.format = Format::A16R16G16B16_SFloat;
			descriptor.usage = IMAGE_USAGE_COLOR_ATTACHMENT | IMAGE_USAGE_SAMPLED;
			descriptor.width = WEngine::Screen::GetWidth();
			descriptor.height = WEngine::Screen::GetHeight();
			descriptor.layout = AttachmentLayout::Undefined;
			descriptor.mipCount = 1;
		}
		RHITextureViewDescriptor viewDescriptor = {};
		{
			viewDescriptor.format = Format::A16R16G16B16_SFloat;
			viewDescriptor.arrayLayerCount = 1;
			viewDescriptor.baseArrayLayer = 0;
			viewDescriptor.mipCount = 1;
			viewDescriptor.baseMipLevel = 0;
			viewDescriptor.dimension = Dimension::Texture2D;
			viewDescriptor.imageAspect = IMAGE_ASPECT_COLOR;
		}
		m_textureResources[i * 2u] = RHIContext::GetDevice()->CreateTexture(&descriptor);
		TextureBarrier textureBarrier = { m_textureResources[i * 2u], AttachmentLayout::Undefined, AttachmentLayout::ColorBuffer, 0, ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE, IMAGE_ASPECT_COLOR };
		RHIBarrierDescriptor barrierDescriptor = {};
		{
			barrierDescriptor.textureCount = 1;
			barrierDescriptor.pTextureBarriers = &textureBarrier;
			barrierDescriptor.srcStage = PIPELINE_STAGE_TOP_OF_PIPE;
			barrierDescriptor.dstStage = PIPELINE_STAGE_EARLY_FRAGMENT_TESTS;
		}
		RHIContext::GetContext()->ResourceBarrier(&barrierDescriptor);
		m_rendertargets[i].pColorTexture = m_textureResources[i * 2u]->CreateTextureView(&viewDescriptor);

		{
			descriptor.format = Format::D16_Unorm;
			descriptor.usage = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT;
		}
		{
			viewDescriptor.format = Format::D16_Unorm;
			viewDescriptor.imageAspect = IMAGE_ASPECT_DEPTH;
		}
		m_textureResources[i * 2u + 1] = RHIContext::GetDevice()->CreateTexture(&descriptor);
		{
			textureBarrier.pTexture = m_textureResources[i * 2u + 1];
			textureBarrier.imageAspect = IMAGE_ASPECT_DEPTH;
			textureBarrier.newLayout = AttachmentLayout::DepthBuffer;
		}
		RHIContext::GetContext()->ResourceBarrier(&barrierDescriptor);
		m_rendertargets[i].pDepthTexture = m_textureResources[i * 2u + 1]->CreateTextureView(&viewDescriptor);
	}

	m_pData = (CameraData*)WEngine::Allocator::Get()->Allocate(sizeof(CameraData));
	::new (m_pData) CameraData();
}

void Camera::ShowInInspector()
{
}

glm::mat4x4 Camera::GetViewMatrix()
{
	UpdateViewMatrix();
	return m_viewMatrix;
}

glm::mat4x4 Camera::GetProjectionMatrix()
{
	UpdateProjectionMatrix();
	return m_projectionMatrix;
}

void Camera::SetRenderer(ScriptableRenderer* renderer)
{
	m_renderer = renderer;
}

ScriptableRenderer* Camera::GetRenderer()
{
	return m_renderer;
}

CameraData* Camera::GetData()
{
	m_pData->camera = this;
	m_pData->Position = m_pGameObject->GetComponent<Transformer>()->GetPosition();
	m_pData->MatrixV = this->GetViewMatrix();
	m_pData->MatrixP = this->GetProjectionMatrix();
	m_pData->MatrixVP = m_pData->MatrixP * m_pData->MatrixV;
	m_pData->pRenderTarget = &m_rendertargets[RHIContext::g_currentFrame];

	return m_pData;
}

void Camera::Move(Direction dir, float dis)
{
	Transformer *transformer = m_pGameObject->GetComponent<Transformer>();
	transformer->Move(dir, dis);
}

void Camera::Rotate(RotateDirection dir, float dis)
{
	Transformer* transformer = m_pGameObject->GetComponent<Transformer>();
	transformer->Rotate(dir, dis);
}

void Camera::RecreateRenderTarget(unsigned int width, unsigned int height)
{
	for (unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
	{
		delete m_rendertargets[i].pColorTexture;
		delete m_rendertargets[i].pDepthTexture;
		delete m_textureResources[i * 2u];
		delete m_textureResources[i * 2u + 1];

		RHITextureDescriptor descriptor = {};
		{
			descriptor.format = Format::A16R16G16B16_SFloat;
			descriptor.usage = IMAGE_USAGE_COLOR_ATTACHMENT | IMAGE_USAGE_SAMPLED;
			descriptor.width = WEngine::Screen::GetWidth();
			descriptor.height = WEngine::Screen::GetHeight();
			descriptor.layout = AttachmentLayout::Undefined;
			descriptor.mipCount = 1;
		}
		RHITextureViewDescriptor viewDescriptor = {};
		{
			viewDescriptor.format = Format::A16R16G16B16_SFloat;
			viewDescriptor.arrayLayerCount = 1;
			viewDescriptor.baseArrayLayer = 0;
			viewDescriptor.mipCount = 1;
			viewDescriptor.baseMipLevel = 0;
			viewDescriptor.dimension = Dimension::Texture2D;
			viewDescriptor.imageAspect = IMAGE_ASPECT_COLOR;
		}
		m_textureResources[i * 2u] = RHIContext::GetDevice()->CreateTexture(&descriptor);
		TextureBarrier textureBarrier = { m_textureResources[i * 2u], AttachmentLayout::Undefined, AttachmentLayout::ColorBuffer, 0, ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE, IMAGE_ASPECT_COLOR };
		RHIBarrierDescriptor barrierDescriptor = {};
		{
			barrierDescriptor.textureCount = 1;
			barrierDescriptor.pTextureBarriers = &textureBarrier;
			barrierDescriptor.srcStage = PIPELINE_STAGE_TOP_OF_PIPE;
			barrierDescriptor.dstStage = PIPELINE_STAGE_EARLY_FRAGMENT_TESTS;
		}
		RHIContext::GetContext()->ResourceBarrier(&barrierDescriptor);
		m_rendertargets[i].pColorTexture = m_textureResources[i * 2u]->CreateTextureView(&viewDescriptor);

		{
			descriptor.format = Format::D16_Unorm;
			descriptor.usage = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT;
		}
		{
			viewDescriptor.format = Format::D16_Unorm;
			viewDescriptor.imageAspect = IMAGE_ASPECT_DEPTH;
		}
		m_textureResources[i * 2u + 1] = RHIContext::GetDevice()->CreateTexture(&descriptor);
		{
			textureBarrier.pTexture = m_textureResources[i * 2u + 1];
			textureBarrier.imageAspect = IMAGE_ASPECT_DEPTH;
			textureBarrier.newLayout = AttachmentLayout::DepthBuffer;
		}
		RHIContext::GetContext()->ResourceBarrier(&barrierDescriptor);
		m_rendertargets[i].pDepthTexture = m_textureResources[i * 2 + 1]->CreateTextureView(&viewDescriptor);
	}

	m_renderer->UpdateRenderTarget(GetData());

	m_aspect = (float)width / (float)height;
}

void Camera::UpdateViewMatrix()
{
	glm::vec3 position = m_pGameObject->GetComponent<Transformer>()->GetPosition();
	glm::vec3 forward = m_pGameObject->GetComponent<Transformer>()->GetForward();
	m_viewMatrix = glm::lookAt(position, position  + forward, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::UpdateProjectionMatrix()
{
	//m_projectionMatrix = Matrix4x4f::GetIdentityMatrix();
	//PerspectiveProjection(m_projectionMatrix, m_fov, m_aspect, m_nearPlane, m_farPlane);
	m_projectionMatrix = glm::perspective(m_fov, m_aspect, m_nearPlane, m_farPlane);
}
