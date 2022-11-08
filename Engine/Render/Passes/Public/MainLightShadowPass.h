#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class MainLightShadowPass : public ScriptableRenderPass
{
public:

	MainLightShadowPass();

	virtual ~MainLightShadowPass();

	virtual void Setup(RHIContext *context, CameraData *cameraData) override;

	virtual void Execute(RHIContext *context, CameraData* cameraData) override;

};