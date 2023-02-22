#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHITexture;
class RHITextureView;
class RHIBuffer;
class RHIGroup;
class RHIPipelineResourceLayout;

class MainLightShadowPass : public ScriptableRenderPass
{
public:

	MainLightShadowPass(ScriptableRenderer* pRenderer);

	virtual ~MainLightShadowPass();

private:

	

private:

	WEngine::WArray<RHIBuffer*> m_pSceneDataBuffers;

	WEngine::WArray<RHIBuffer*> m_pObjectDataBuffers;

	WEngine::WArray<RHIGroup*> m_pDataGroup;

	RHIPipelineResourceLayout *m_pPipelineLayout;

};