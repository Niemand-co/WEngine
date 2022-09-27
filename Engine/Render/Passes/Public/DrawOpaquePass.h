#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class DrawOpaquePass : public ScriptableRenderPass
{
public:

	DrawOpaquePass(RenderPassConfigure *configure);

	virtual ~DrawOpaquePass();

	virtual void Setup() override;

	virtual void Execute(RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence *fence) override;

};