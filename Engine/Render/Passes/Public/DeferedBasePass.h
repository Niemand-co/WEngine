#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class DeferedBasePass : public ScriptableRenderPass
{
public:

	DeferedBasePass();

	virtual ~DeferedBasePass();

	virtual void Setup(RHIContext* context, CameraData *data) override;

	virtual void Execute(RHIContext* context, CameraData *data) override;

};