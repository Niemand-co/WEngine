#pragma once

namespace WEngine
{

	class CascadedShadowMap
	{
	public:

		static std::vector<glm::mat4> GetPSSMMatrices(glm::mat4 cameraMatrixInv, float nearClip, float farClip, glm::vec3 lightDir);

	};

}