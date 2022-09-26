#include "pch.h"
#include "Render/Passes/Public/FinalBlitPass.h"

FinalBlitPass::FinalBlitPass(RenderPassConfigure* configure)
	: ScriptableRenderPass(configure)
{
}

FinalBlitPass::~FinalBlitPass()
{
}

void FinalBlitPass::Setup()
{
}

void FinalBlitPass::Execute(RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore)
{
}
