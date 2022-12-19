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

	virtual void Setup(RHIContext *context, CameraData *cameraData) override;

	virtual void Execute(RHIContext *context, CameraData* cameraData) override;

	virtual void UpdateRenderTarget(CameraData* cameraData) override;

private:

	

private:

	WEngine::WArray<RHIBuffer*> m_pSceneDataBuffers;

	WEngine::WArray<RHIBuffer*> m_pObjectDataBuffers;

	WEngine::WArray<RHIGroup*> m_pDataGroup;

	RHIPipelineResourceLayout *m_pPipelineLayout;

};