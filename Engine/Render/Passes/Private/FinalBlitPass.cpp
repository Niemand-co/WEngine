#include "pch.h"
#include "Render/Passes/Public/FinalBlitPass.h"

FinalBlitPass::FinalBlitPass(ScriptableRenderer* pRenderer)
	: ScriptableRenderPass(pRenderer)
{
}

FinalBlitPass::~FinalBlitPass()
{
}

void FinalBlitPass::Setup(RHIContext *context, CameraData *cameraData)
{
}

void FinalBlitPass::Execute(RHIContext *context, CameraData* cameraData)
{
}
