#pragma once

namespace WEngine
{

	class CascadedShadowMap
	{
	public:

		static std::vector<glm::mat4> GetPSSMMatrices(glm::vec3 viewPos, glm::vec3 viewDir, glm::vec3 lightDir, float fov, float aspect, const std::vector<float>& splices);

	};

}