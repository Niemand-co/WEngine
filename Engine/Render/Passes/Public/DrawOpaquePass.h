#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHIRenderTarget;
class Mesh;
class RHIBuffer;

class DrawOpaquePass : public ScriptableRenderPass
{
public:

	DrawOpaquePass(RenderPassConfigure *configure);

	virtual ~DrawOpaquePass();

	virtual void Setup(RHIContext *context) override;

	virtual void Execute(RHIContext *context, RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence *fence) override;

private:

	std::vector<RHIRenderTarget*> m_pRenderTargets;

	Mesh *m_pMesh;

	RHIBuffer *m_pBuffer;

};