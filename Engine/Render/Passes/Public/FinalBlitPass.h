#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class FinalBlitPass : public ScriptableRenderPass
{
public:

	FinalBlitPass(RenderPassConfigure *configure);

	virtual ~FinalBlitPass();

	virtual void Setup(RHIContext *context, CameraData *cameraData) override;

	virtual void Execute(RHIContext *context) override;

};