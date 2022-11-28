#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHIPipelineStateObject;
class RHISampler;
class GameObject;

class DrawGUIPass : public ScriptableRenderPass
{
public:

	DrawGUIPass(ScriptableRenderer* pRenderer);

	virtual ~DrawGUIPass();

	virtual void Setup(RHIContext *context, CameraData *cameraData) override;

	virtual void Execute(RHIContext *context, CameraData* cameraData) override;

	virtual void UpdateRenderTarget(CameraData *cameraData) override;

private:

	GameObject *m_currentGo;

	RHISampler *m_pSampler;

};