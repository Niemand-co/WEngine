#include "pch.h"
#include "Scene/Components/Public/Light.h"
#include "Scene/Public/GameObject.h"
#include "Scene/Public/World.h"
#include "RHI/Public/RHIHeads.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Scene/Components/Public/Camera.h"

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
	m_mainLightCascadedShadowMapRange[0] = 0.01f;
	m_mainLightCascadedShadowMapRange[1] = 25.0f;
	m_mainLightCascadedShadowMapRange[2] = 50.0f;
	m_mainLightCascadedShadowMapRange[3] = 75.0f;
	m_mainLightCascadedShadowMapRange[4] = 100.0f;
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
	std::vector<glm::mat4> shadowFrustum(m_mainLightCascadedShadowMapNum);

	Transformer* pTransformer = cameraData->camera->GetGameObject()->GetComponent<Transformer>();
	glm::vec3 center = cameraData->Position;
	glm::vec3 forward = pTransformer->GetForward();
	glm::mat4 lightSpaceMatrix = World::GetWorld()->GetMainLight()->GetGameObject()->GetComponent<Transformer>()->GetWorldToLocalMatrix();
	glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 up = glm::normalize(glm::cross(right, forward));

	for (unsigned int i = 0; i < m_mainLightCascadedShadowMapNum; ++i)
	{
		float half_fov = glm::radians(cameraData->fov / 2.0f);
		float div_aspect = 1.0f / cameraData->aspect;

		float near_height = half_fov * m_mainLightCascadedShadowMapRange[i];
		float near_width = near_height * div_aspect;

		float far_height = half_fov * m_mainLightCascadedShadowMapRange[i + 1u];
		float far_width = far_height * div_aspect;

		glm::vec3 near_center = center + m_mainLightCascadedShadowMapRange[i] * forward;
		glm::vec3 far_center = center + m_mainLightCascadedShadowMapRange[i + 1u] * forward;

		glm::vec3 frustum[8];
		frustum[0] = near_center + right * near_width + up * near_height;
		frustum[1] = near_center + right * near_width - up * near_height;
		frustum[2] = near_center - right * near_width - up * near_height;
		frustum[3] = near_center - right * near_width + up * near_height;

		frustum[4] = far_center + right * far_width + up * far_height;
		frustum[5] = far_center + right * far_width - up * far_height;
		frustum[6] = far_center - right * far_width - up * far_height;
		frustum[7] = far_center - right * far_width + up * far_height;

		float nearP = (std::numeric_limits<float>::max)();
		float farP = (std::numeric_limits<float>::min)();
		float left = (std::numeric_limits<float>::max)();
		float right = (std::numeric_limits<float>::min)();
		float bottom = (std::numeric_limits<float>::max)();
		float top = (std::numeric_limits<float>::min)();
		for (unsigned int j = 0; j < 8; ++j)
		{
			frustum[j] = lightSpaceMatrix * glm::vec4(frustum[j], 1.0f);
			nearP = frustum[j].z < nearP ? frustum[j].z : nearP;
			farP = frustum[j].z > farP ? frustum[j].z : farP;
			left = frustum[j].x < left ? frustum[j].x : left;
			right = frustum[j].x > right ? frustum[j].x : right;
			bottom = frustum[j].y < bottom ? frustum[j].y : bottom;
			top = frustum[j].y > top ? frustum[j].y : top;
		}

		shadowFrustum[i] = glm::ortho(left, right, bottom, top, nearP, farP);
	}

	return shadowFrustum;
}