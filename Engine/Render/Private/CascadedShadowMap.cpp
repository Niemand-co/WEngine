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

			glm::mat4 lightViewMatrix = glm::lookAt(glm::vec3(), lightDir, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 lightViewMatrixInv = glm::inverse(lightViewMatrix);
			for (unsigned int j = 0; j < 8; ++j)
			{
				box[j] = lightViewMatrix * glm::vec4(box[j], 1.0f);
			}

			glm::vec3 MinCorner = box[0];
			glm::vec3 MaxCorner = box[0];
			for (unsigned int j = 1; j < 8; ++j)
			{
				MinCorner.x = MinCorner.x > box[j].x ? box[j].x : MinCorner.x;
				MinCorner.y = MinCorner.y > box[j].y ? box[j].y : MinCorner.y;
				MinCorner.z = MinCorner.z > box[j].z ? box[j].z : MinCorner.z;

				MaxCorner.x = MaxCorner.x < box[j].x ? box[j].x : MaxCorner.x;
				MaxCorner.y = MaxCorner.y < box[j].y ? box[j].y : MaxCorner.y;
				MaxCorner.z = MaxCorner.z < box[j].z ? box[j].z : MaxCorner.z;
			}

			glm::vec3 lightSpaceBox[8];
			{
				lightSpaceBox[0] = { MinCorner.x, MinCorner.y, MinCorner.z };
				lightSpaceBox[1] = { MaxCorner.x, MinCorner.y, MinCorner.z };
				lightSpaceBox[2] = { MaxCorner.x, MinCorner.y, MaxCorner.z };
				lightSpaceBox[3] = { MinCorner.x, MinCorner.y, MaxCorner.z };
				lightSpaceBox[4] = { MinCorner.x, MaxCorner.y, MinCorner.z };
				lightSpaceBox[5] = { MaxCorner.x, MaxCorner.y, MinCorner.z };
				lightSpaceBox[6] = { MaxCorner.x, MaxCorner.y, MaxCorner.z };
				lightSpaceBox[7] = { MinCorner.x, MaxCorner.y, MaxCorner.z };
			}

			glm::vec3 center = glm::vec3();
			for (unsigned int j = 0; j < 8; ++j)
			{
				lightSpaceBox[j] = lightViewMatrixInv * glm::vec4(lightSpaceBox[j], 1.0f);
				center += lightSpaceBox[j];
			}
			center *= 0.125f;

			center = (lightSpaceBox[0] + lightSpaceBox[6]) / 2.0f;
			float width = glm::length(lightSpaceBox[1] - lightSpaceBox[0]);
			float height = glm::length(lightSpaceBox[4] - lightSpaceBox[0]);
			float len = width > height ? width : height;
			len *= 0.5f;
			float distance = 500.0f;

			lightViewMatrix = glm::lookAt(center, lightDir, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 lightProjectionMatrix = glm::ortho(-len, len, -len, len, -distance, distance);

			matrices[i] = lightProjectionMatrix * lightViewMatrix;
		}

		return matrices;

	}

}