#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class DrawGUIPass : public ScriptableRenderPass
{
public:

	DrawGUIPass(RenderPassConfigure* pConfigure);

	virtual ~DrawGUIPass();

	virtual void Setup(RHIContext *context, CameraData *cameraData);

	virtual void Execute(RHIContext *context, RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore, RHIFence *fence = nullptr);

private:

	ImGuiIO m_io;

};