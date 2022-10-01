#include "pch.h"
#include "Render/Passes/Public/MainLightShadowPass.h"

MainLightShadowPass::MainLightShadowPass(RenderPassConfigure* configure)
	: ScriptableRenderPass(configure)
{
}

MainLightShadowPass::~MainLightShadowPass()
{
}

void MainLightShadowPass::Setup(RHIContext *context)
{
}

void MainLightShadowPass::Execute(RHIContext *context, RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence *fence)
{
}
