#include "pch.h"
#include "Render/Public/CascadedShadowMap.h"
#include "Editor/Public/Debug.h"

namespace WEngine
{

	std::vector<glm::mat4> CascadedShadowMap::GetPSSMMatrices(glm::vec3 viewPos, glm::vec3 viewDir, glm::vec3 lightDir, float fov, float aspect, const std::vector<float>& splices)
	{
		std::vector<glm::mat4> matrices(splices.size());

		glm::vec3 right = glm::normalize(glm::cross(viewDir, glm::vec3(0.0f, 1.0f, 0.0f)));
		glm::vec3 up = glm::normalize(glm::cross(right, viewDir));

		for (unsigned int i = 0; i < splices.size() - 1; ++i)
		{
			glm::vec3 box[8];

			float half_fov = glm::radians(fov / 2.0f);
			float div_aspect = 1.0f / aspect;

			float near_height = half_fov * splices[i];
			float near_width = near_height * div_aspect;

			float far_height = half_fov * splices[i + 1u];
			float far_width = far_height * div_aspect;

			glm::vec3 near_center = viewPos + splices[i] * viewDir;
			glm::vec3 far_center = viewPos + splices[i + 1u] * viewDir;

			box[0] = near_center + right * near_width + up * near_height;
			box[1] = near_center + right * near_width - up * near_height;
			box[2] = near_center - right * near_width - up * near_height;
			box[3] = near_center - right * near_width + up * near_height;

			box[4] = far_center + right * far_width + up * far_height;
			box[5] = far_center + right * far_width - up * far_height;
			box[6] = far_center - right * far_width - up * far_height;
			box[7] = far_center - right * far_width + up * far_height;

			glm::vec3 minBox = box[0];
			glm::vec3 maxBox = box[0];
			for (unsigned int j = 1; j < 8; ++j)
			{
				minBox.x = minBox.x > box[j].x ? box[j].x : minBox.x;
				minBox.y = minBox.y > box[j].y ? box[j].y : minBox.y;
				minBox.z = minBox.z > box[j].z ? box[j].z : minBox.z;
				maxBox.x = maxBox.x < box[j].x ? box[j].x : maxBox.x;
				maxBox.y = maxBox.y < box[j].y ? box[j].y : maxBox.y;
				maxBox.z = maxBox.z < box[j].z ? box[j].z : maxBox.z;
			}

			box[0] = glm::vec3(minBox.x, minBox.y, minBox.z);
			box[1] = glm::vec3(maxBox.x, minBox.y, minBox.z);
			box[2] = glm::vec3(maxBox.x, minBox.y, maxBox.z);
			box[3] = glm::vec3(minBox.x, minBox.y, maxBox.z);
			box[4] = glm::vec3(minBox.x, maxBox.y, minBox.z);
			box[5] = glm::vec3(maxBox.x, maxBox.y, minBox.z);
			box[6] = glm::vec3(maxBox.x, maxBox.y, maxBox.z);
			box[7] = glm::vec3(minBox.x, maxBox.y, minBox.z);

			glm::vec3 lightRight = glm::cross(lightDir, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec3 lightUp = glm::cross(lightRight, lightDir);
			glm::mat3 lightSpaceMatrix = glm::mat3(lightRight, lightUp, lightDir);

			for (unsigned int j = 0; j < 8; ++j)
			{
				box[j] = lightSpaceMatrix * box[j];
			}

			minBox = box[0];
			maxBox = box[0];
			for (unsigned int j = 1; j < 8; ++j)
			{
				minBox.x = minBox.x > box[j].x ? box[j].x : minBox.x;
				minBox.y = minBox.y > box[j].y ? box[j].y : minBox.y;
				minBox.z = minBox.z > box[j].z ? box[j].z : minBox.z;
				maxBox.x = maxBox.x < box[j].x ? box[j].x : maxBox.x;
				maxBox.y = maxBox.y < box[j].y ? box[j].y : maxBox.y;
				maxBox.z = maxBox.z < box[j].z ? box[j].z : maxBox.z;
			}

			//glm::mat4 projectionMatrix = glm::mat4(glm::vec4(2.0f / (maxBox.x - minBox.x), 0.0f, 0.0f, (maxBox.x + minBox.x) / (maxBox.x - minBox.x)),
			//									   glm::vec4(0.0f, 2.0f / (maxBox.y - minBox.y), 0.0f, (maxBox.y + minBox.y) / (maxBox.y - minBox.y)),
			//									   glm::vec4(0.0f, 0.0f, 2.0f / (maxBox.z - minBox.z), (maxBox.z + minBox.z) / (maxBox.z - minBox.z)),
			//									   glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

			glm::mat4 projectionMatrix = glm::ortho(minBox.x, maxBox.x, minBox.y, maxBox.y, minBox.z - 100.0f, maxBox.z);

			matrices[i] = projectionMatrix * glm::mat4(lightSpaceMatrix);
		}

		return matrices;

	}

}