#pragma once
#include "Render/Public/RenderTexture.h"

namespace WEngine
{

	class CascadedShadowMap
	{
	public:

		static void UpdateSplices(float *splices, unsigned int spliceCount, float nearClip, float farClip);

		static void UpdatePSSMMatrices(WEngine::WArray<glm::mat4>& matrices, glm::mat4 cameraMatrixInv, glm::vec3 lightDir, float *splices, unsigned int spliceCount);

	};

	struct CSMShadowMapPack
	{
		WArray<ShadowMap2D> ShadowMaps;
		WArray<float> SpliceDistances;
		WArray<glm::mat4> SpliceMatrices;
	};

}