#pragma once
#include "Render/Public/RenderTexture.h"

struct CSMShadowMapPack
{
	WEngine::WArray<ShadowMap2D> ShadowMaps;
	WEngine::WArray<float> SpliceDistances;
	WEngine::WArray<glm::mat4> SpliceMatrices;
};

namespace WEngine
{

	class CascadedShadowMap
	{
	public:

		static void UpdateSplices(CSMShadowMapPack& outPack, float nearClip, float farClip);

		static void UpdatePSSMMatrices(CSMShadowMapPack& outPack, glm::mat4 cameraMatrixInv, glm::vec3 lightDir);

	};


}