#include "pch.h"
#include "Scene/Components/Public/Light.h"
#include "Scene/Public/GameObject.h"
#include "Scene/Public/World.h"
#include "RHI/Public/RHIHeads.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Scene/Components/Public/Camera.h"
#include "Render/Public/CascadedShadowMap.h"

Light::Light(GameObject *pGameObject)
	: Component(pGameObject), m_type(LightType::Directional), m_intensity(1.0f), m_color(glm::vec3(1.0f, 1.0f, 1.0f))
{
	World::GetWorld()->AddLight(this);

	m_pDepthTextures.resize(RHIContext::g_maxFrames);
	m_pDepthTextureViews.resize(RHIContext::g_maxFrames);

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
	}

	m_mainLightCascadedShadowMapRange.resize(m_mainLightCascadedShadowMapNum + 1);
	for (unsigned int i = 1; i <= m_mainLightCascadedShadowMapNum; ++i)
	{
		float ratio = (float)i / (float)m_mainLightCascadedShadowMapNum;
		float logC = 0.01 * std::pow(10000.0f, ratio);
		float uniC = 0.01f + (1000.0f - 0.01f) * ratio;
		m_mainLightCascadedShadowMapRange[i] = 0.75f * logC + 0.25f * uniC;
	}
}

Light::~Light()
{
}

void Light::SetMainLight(bool isMainLight)
{
	m_isMainLight = isMainLight;
}

void Light::ShowInInspector()
{
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Intensity"); ImGui::SameLine();
		ImGui::SliderFloat("##Intensity", &m_intensity, 0.0f, 10.0f);

		ImGui::Text("Color"); ImGui::SameLine();
		ImGui::ColorEdit3("##LightColor", &m_color[0]);
	}
}

std::vector<glm::mat4> Light::GetShadowFrustum(CameraData* cameraData)
{
	Transformer* pTransformer = cameraData->camera->GetGameObject()->GetComponent<Transformer>();

	return WEngine::CascadedShadowMap::GetPSSMMatrices(cameraData->Position, pTransformer->GetForward(), -m_pGameObject->GetComponent<Transformer>()->GetForward(), cameraData->fov, cameraData->aspect, m_mainLightCascadedShadowMapRange);
}