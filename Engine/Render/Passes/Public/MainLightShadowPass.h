#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHITexture;
class RHITextureView;
class RHIBuffer;
class RHIGroup;

class MainLightShadowPass : public ScriptableRenderPass
{
public:

	MainLightShadowPass();

	virtual ~MainLightShadowPass();

	virtual void Setup(RHIContext *context, CameraData *cameraData) override;

	virtual void Execute(RHIContext *context, CameraData* cameraData) override;

	virtual void UpdateRenderTarget(CameraData* cameraData) override;

private:

	std::vector<RHITexture*> m_pDepthTextures;

	std::vector<RHITextureView*> m_pDepthTextureViews;

	std::vector<RHIBuffer*> m_pSceneDataBuffers;

	std::vector<RHIBuffer*> m_pObjectDataBuffers;

	unsigned int dynamicBufferAlignment;

	std::vector<RHIGroup*> m_pDataGroup;

};