#include "pch.h"
#include "Render/Passes/Public/FinalBlitPass.h"

FinalBlitPass::FinalBlitPass(RenderPassConfigure* configure)
	: ScriptableRenderPass(configure)
{
}

FinalBlitPass::~FinalBlitPass()
{
}

void FinalBlitPass::Setup(RHIContext *context, CameraData *cameraData)
{
}

void FinalBlitPass::Execute(RHIContext *context, RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence *fence, RHIEvent* pEvent)
{
}
