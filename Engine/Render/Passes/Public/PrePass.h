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

	virtual void Setup(RHIContext *context, CameraData *cameraData);

	virtual void Execute(RHIContext *context, CameraData* cameraData);

	virtual void UpdateRenderTarget(CameraData *cameraData);

private:

	std::vector<RHITexture*> m_pDepthTextures;

	std::vector<RHITextureView*> m_pDepthTextureViews;

	std::vector<RHIBuffer*> m_pSceneDataBuffers;

	std::vector<RHIBuffer*> m_pObjectDataBuffers;

	std::vector<RHIGroup*> m_pDataGroup;

	RHIPipelineResourceLayout* m_pPipelineLayout;

};