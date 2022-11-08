#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHIRenderTarget;
class RHIPipelineStateObject;
class RHISampler;
class GameObject;

class DrawGUIPass : public ScriptableRenderPass
{
public:

	DrawGUIPass();

	virtual ~DrawGUIPass();

	virtual void Setup(RHIContext *context, CameraData *cameraData) override;

	virtual void Execute(RHIContext *context, CameraData* cameraData) override;

private:

	std::vector<RHIRenderTarget*> m_pRenderTargets;

	RHIPipelineStateObject *m_pPSO;

	GameObject *m_currentGo;

	RHISampler *m_pSampler;

};