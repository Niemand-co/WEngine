#include "pch.h"
#include "Render/Passes/Public/MainLightShadowPass.h"

MainLightShadowPass::MainLightShadowPass(RenderPassConfigure* configure)
	: ScriptableRenderPass(configure)
{
}

MainLightShadowPass::~MainLightShadowPass()
{
}

void MainLightShadowPass::Setup()
{
}

void MainLightShadowPass::Execute(RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence *fence)
{
}
