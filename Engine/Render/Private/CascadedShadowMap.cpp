#include "pch.h"
#include "Render/Public/CascadedShadowMap.h"

namespace WEngine
{

	std::vector<glm::mat4> CascadedShadowMap::GetPSSMMatrices(glm::vec3 viewPos, glm::vec3 viewDir, glm::vec3 lightDir, float fov, float aspect, const std::vector<float>& splices)
	{
		std::vector<glm::mat4> matrices(splices.size());

		glm::vec3 right = glm::normalize(glm::cross(viewDir, glm::vec3(0.0f, 1.0f, 0.0f)));
		glm::vec3 up = glm::normalize(glm::cross(right, viewDir));

		for (unsigned int i = 0; i < splices.size(); ++i)
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
		}

	}

}