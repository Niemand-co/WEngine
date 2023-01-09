#include "pch.h"
#include "Scene/Components/Public/LightComponent.h"
#include "Scene/Public/GameObject.h"
#include "Scene/Public/World.h"
#include "RHI/Public/RHIHeads.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Scene/Components/Public/CameraComponent.h"
#include "Render/Public/CascadedShadowMap.h"

LightComponent::LightComponent(GameObject *pGameObject)
	: Component(pGameObject), m_type(LightType::Directional), m_intensity(1.0f), m_color(glm::vec3(1.0f, 1.0f, 1.0f))
{
	World::GetWorld()->AddLight(this);

	m_mainLightCascadedShadowMapNum = 4;

	m_pDepthTextures.Resize(RHIContext::g_maxFrames * m_mainLightCascadedShadowMapNum);
	m_pDepthTextureViews.Resize(RHIContext::g_maxFrames * m_mainLightCascadedShadowMapNum);
	m_mainLightCascadedShadowMapRange.Resize(m_mainLightCascadedShadowMapNum);
	m_lightSpaceMatrix.Resize(m_mainLightCascadedShadowMapNum);

	RHITextureDescriptor textureDescriptor = {};
	{
		textureDescriptor.format = Format::D32_SFloat;
		textureDescriptor.width = 1024;
		textureDescriptor.height = 1024;
		textureDescriptor.layerCount = 1;
		textureDescriptor.mipCount = 1;
		textureDescriptor.usage = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT | IMAGE_USAGE_SAMPLED;
		textureDescriptor.layout = AttachmentLayout::Undefined;
	}

	RHITextureViewDescriptor textureViewDescriptor = {};
	{
		textureViewDescriptor.format = Format::D32_SFloat;
		textureViewDescriptor.dimension = Dimension::Texture2D;
		textureViewDescriptor.arrayLayerCount = 1;
		textureViewDescriptor.baseArrayLayer = 0;
		textureViewDescriptor.mipCount = 1;
		textureViewDescriptor.baseMipLevel = 0;
		textureViewDescriptor.imageAspect = IMAGE_ASPECT_DEPTH;
	}

	WEngine::WArray<TextureBarrier> barriers(m_pDepthTextures.Size());
	for (unsigned int i = 0; i < m_pDepthTextures.Size(); ++i)
	{
		m_pDepthTextures[i] = RHIContext::GetDevice()->CreateTexture(&textureDescriptor);
		barriers[i] = { m_pDepthTextures[i], AttachmentLayout::Undefined, AttachmentLayout::DepthBuffer, 0, ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE, IMAGE_ASPECT_DEPTH };
	}

	RHIBarrierDescriptor barrierDescriptor = {};
	{
		barrierDescriptor.textureCount = barriers.Size();
		barrierDescriptor.pTextureBarriers = barriers.GetData();
		barrierDescriptor.srcStage = PIPELINE_STAGE_TOP_OF_PIPE;
		barrierDescriptor.dstStage = PIPELINE_STAGE_EARLY_FRAGMENT_TESTS;
	}
	RHIContext::GetContext()->ResourceBarrier(&barrierDescriptor);

	for (unsigned int i = 0; i < m_pDepthTextureViews.Size(); ++i)
	{
		m_pDepthTextureViews[i] = m_pDepthTextures[i]->CreateTextureView(&textureViewDescriptor);
	}

}

LightComponent::~LightComponent()
{
}

void LightComponent::SetMainLight(bool isMainLight)
{
	m_isMainLight = isMainLight;
}

void LightComponent::ShowInInspector()
{
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Intensity"); ImGui::SameLine();
		ImGui::SliderFloat("##Intensity", &m_intensity, 0.0f, 10.0f);

		ImGui::Text("Color"); ImGui::SameLine();
		ImGui::ColorEdit3("##LightColor", &m_color[0]);
	}
}

void LightComponent::UpdateShadowFrustum(CameraData* cameraData)
{
	WEngine::CascadedShadowMap::UpdateSplices(m_mainLightCascadedShadowMapRange.GetData(), m_mainLightCascadedShadowMapRange.Size(), cameraData->nearClip, cameraData->farClip);
	WEngine::CascadedShadowMap::UpdatePSSMMatrices(m_lightSpaceMatrix, glm::inverse(cameraData->MatrixVP), m_pGameObject->GetComponent<Transformer>()->GetForward(), m_mainLightCascadedShadowMapRange.GetData(), m_mainLightCascadedShadowMapNum);
}

const WEngine::WArray<glm::mat4>& LightComponent::GetShadowFrustum()
{
	return m_lightSpaceMatrix;
}

const WEngine::WArray<float>& LightComponent::GetSplices()
{
	return m_mainLightCascadedShadowMapRange;
}
