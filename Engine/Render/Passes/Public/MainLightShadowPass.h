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

	std::vector<RHIBuffer*> m_pSceneDataBuffers;

	std::vector<RHIBuffer*> m_pObjectDataBuffers;

	std::vector<RHIGroup*> m_pDataGroup;

	RHIPipelineResourceLayout *m_pPipelineLayout;

};