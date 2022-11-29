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
	for (unsigned int i = 0; i < m_mainLightCascadedShadowMapNum; ++i)
	{
		float ratio = (float)(i + 1) / (float)m_mainLightCascadedShadowMapNum;
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
	std::vector<glm::mat4> shadowFrustum(m_mainLightCascadedShadowMapNum);

	Transformer* pTransformer = cameraData->camera->GetGameObject()->GetComponent<Transformer>();


	for (unsigned int i = 0; i < m_mainLightCascadedShadowMapNum; ++i)
	{


		glm::vec3 maxBox = frustum[0];
		glm::vec3 minBox = frustum[0];
		for (unsigned int j = 1; j < 8; ++j)
		{
			maxBox.x = maxBox.x < frustum[i].x ? frustum[i].x : maxBox.x;
			maxBox.y = maxBox.y < frustum[i].y ? frustum[i].y : maxBox.y;
			maxBox.z = maxBox.z < frustum[i].z ? frustum[i].z : maxBox.z;
			minBox.x = minBox.x > frustum[i].x ? frustum[i].x : minBox.x;
			minBox.y = minBox.y > frustum[i].y ? frustum[i].y : minBox.y;
			minBox.z = minBox.z > frustum[i].z ? frustum[i].z : minBox.z;
		}

		frustum[0] = lightSpaceMatrix * glm::vec4(minBox.x, minBox.y, minBox.z, 1.0);
		frustum[1] = lightSpaceMatrix * glm::vec4(maxBox.x, minBox.y, minBox.z, 1.0);
		frustum[2] = lightSpaceMatrix * glm::vec4(maxBox.x, minBox.y, maxBox.z, 1.0);
		frustum[3] = lightSpaceMatrix * glm::vec4(minBox.x, minBox.y, maxBox.z, 1.0);
		frustum[4] = lightSpaceMatrix * glm::vec4(minBox.x, maxBox.y, minBox.z, 1.0);
		frustum[5] = lightSpaceMatrix * glm::vec4(maxBox.x, maxBox.y, minBox.z, 1.0);
		frustum[6] = lightSpaceMatrix * glm::vec4(maxBox.x, maxBox.y, maxBox.z, 1.0);
		frustum[7] = lightSpaceMatrix * glm::vec4(minBox.x, maxBox.y, maxBox.z, 1.0);

		for (unsigned int j = 1; j < 8; ++j)
		{
			maxBox.x = maxBox.x < frustum[i].x ? frustum[i].x : maxBox.x;
			maxBox.y = maxBox.y < frustum[i].y ? frustum[i].y : maxBox.y;
			maxBox.z = maxBox.z < frustum[i].z ? frustum[i].z : maxBox.z;
			minBox.x = minBox.x > frustum[i].x ? frustum[i].x : minBox.x;
			minBox.y = minBox.y > frustum[i].y ? frustum[i].y : minBox.y;
			minBox.z = minBox.z > frustum[i].z ? frustum[i].z : minBox.z;
		}

		//minBox = glm::inverse(lightSpaceMatrix) * glm::vec4(minBox, 1.0f);
		//maxBox = glm::inverse(lightSpaceMatrix) * glm::vec4(maxBox, 1.0f);

		shadowFrustum[i] = glm::ortho(0.f, maxBox.x - minBox.x, 0.f, maxBox.y - minBox.y, minBox.z - maxBox.z, maxBox.z - minBox.z);
	}

	return shadowFrustum;
}