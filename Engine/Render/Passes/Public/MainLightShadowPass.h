#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class MainLightShadowPass : public ScriptableRenderPass
{
public:

	MainLightShadowPass(RenderPassConfigure *configure);

	virtual ~MainLightShadowPass();

	virtual void Setup() override;

	virtual void Execute(RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence *fence) override;

};