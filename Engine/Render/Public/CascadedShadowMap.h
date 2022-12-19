#pragma once

namespace WEngine
{

	class CascadedShadowMap
	{
	public:

		static void UpdateSplices(float *splices, unsigned int spliceCount, float nearClip, float farClip);

		static void UpdatePSSMMatrices(WEngine::WArray<glm::mat4>& matrices, glm::mat4 cameraMatrixInv, glm::vec3 lightDir, float *splices, unsigned int spliceCount);

	};

}