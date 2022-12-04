#include "pch.h"
#include "Render/Public/CascadedShadowMap.h"
#include "Editor/Public/Debug.h"
#include "Scene/Public/GameObject.h"

namespace WEngine
{

	std::vector<glm::mat4> CascadedShadowMap::GetPSSMMatrices(glm::mat4 cameraMatrixInv, float nearClip, float farClip, glm::vec3 lightDir)
	{
		std::vector<glm::mat4> matrices(4);

		float splices[4];
		for (unsigned int i = 0; i < 4; ++i)
		{
			float ratio = (float)(i + 1) / 4.0;
			float logC = nearClip * std::pow(farClip / nearClip, ratio);
			float uniC = 0.01f + (farClip - nearClip) * ratio;
			float d = 0.95f * logC + 0.05f * uniC;
			splices[i] = (d - nearClip) / (farClip - nearClip);
		}

		for (unsigned int i = 0; i < 4; ++i)
		{
			glm::vec3 box[8];
			{
				box[0] = glm::vec3(-1.0f, -1.0f, -1.0f);
				box[1] = glm::vec3(1.0f, -1.0f, -1.0f);
				box[2] = glm::vec3(1.0f, 1.0f, -1.0f);
				box[3] = glm::vec3(-1.0f, 1.0f, -1.0f);
				box[4] = glm::vec3(-1.0f, -1.0f, 1.0f);
				box[5] = glm::vec3(1.0f, -1.0f, 1.0f);
				box[6] = glm::vec3(1.0f, 1.0f, 1.0f);
				box[7] = glm::vec3(-1.0f, 1.0f, 1.0f);
			}

			for (unsigned int j = 0; j < 8; ++j)
			{
				glm::vec4 clipPos = cameraMatrixInv * glm::vec4(box[j], 1.0f);
				box[j] = glm::vec3(clipPos) / clipPos.w;
			}

			for (unsigned int j = 0; j < 4; ++j)
			{
				glm::vec3 frustumDir = box[j + 4] - box[j];
				box[j + 4] = splices[j] * frustumDir + box[j];
			}

			glm::vec3 frustumCenter = glm::vec3();
			for (unsigned int j = 0; j < 8; ++j)
			{
				frustumCenter += box[j];
			}
			frustumCenter *= 0.125f;

			float radius = 0.0f;
			for (unsigned int j = 0; j < 8; ++j)
			{
				glm::vec3 disDir = box[j] - frustumCenter;
				float distance = glm::dot(disDir, disDir);
				radius = radius < distance ? distance : radius;
			}
			radius = std::ceil(std::sqrt(radius));

			glm::mat4 lightViewMatrix = glm::lookAt(frustumCenter - radius * lightDir, frustumCenter, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 lightProjectionMatrix = glm::ortho(-radius, radius, -radius, radius, 0.0f, 2.0f * radius);

			matrices[i] = lightProjectionMatrix * lightViewMatrix;
		}

		return matrices;

	}

}