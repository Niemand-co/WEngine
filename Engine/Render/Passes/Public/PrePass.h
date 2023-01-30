#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHITexture;
class RHITextureView;
class RHIBuffer;
class RHIGroup;
class RHIPipelineResourceLayout;

class PrePass : public ScriptableRenderPass
{
public:

	PrePass(ScriptableRenderer* pRenderer);

	virtual ~PrePass();

	virtual void Setup();

	virtual void Execute();

private:

	WEngine::WArray<RHITexture*> m_pDepthTextures;

	WEngine::WArray<RHITextureView*> m_pDepthTextureViews;

	WEngine::WArray<RHIBuffer*> m_pSceneDataBuffers;

	WEngine::WArray<RHIBuffer*> m_pObjectDataBuffers;

	WEngine::WArray<RHIGroup*> m_pDataGroup;

	RHIPipelineResourceLayout* m_pPipelineLayout;

};