#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHIRenderTarget;
class RHIPipelineStateObject;

class DrawGUIPass : public ScriptableRenderPass
{
public:

	DrawGUIPass(RenderPassConfigure* pConfigure);

	virtual ~DrawGUIPass();

	virtual void Setup(RHIContext *context, CameraData *cameraData);

	virtual void Execute(RHIContext *context, RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore, RHIFence *fence = nullptr);

private:

	std::vector<RHIRenderTarget*> m_pRenderTargets;

	RHIPipelineStateObject *m_pPSO;

};