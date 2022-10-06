#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHIRenderTarget;
class Mesh;
class RHIBuffer;
class RHIGroup;
class RHIPipelineResourceLayout;

class DrawOpaquePass : public ScriptableRenderPass
{
public:

	DrawOpaquePass(RenderPassConfigure *configure);

	virtual ~DrawOpaquePass();

	virtual void Setup(RHIContext *context, CameraData *cameraData) override;

	virtual void Execute(RHIContext *context, RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence *fence) override;

private:

	std::vector<RHIRenderTarget*> m_pRenderTargets;

	Mesh *m_pMesh;

	RHIBuffer *m_pVertexBuffer;

	RHIBuffer *m_pIndexBuffer;

	RHIBuffer *m_pUniformBuffer;

	RHIPipelineResourceLayout *m_pPipelineResourceLayout;

	RHIGroup *m_pGroup;

};