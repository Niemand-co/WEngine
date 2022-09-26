#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class FinalBlitPass : public ScriptableRenderPass
{
public:

	FinalBlitPass(RenderPassConfigure *configure);

	virtual ~FinalBlitPass();

	virtual void Setup() override;

	virtual void Execute(RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore) override;

};