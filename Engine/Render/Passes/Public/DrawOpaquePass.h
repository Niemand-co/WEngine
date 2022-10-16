#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHIRenderTarget;
class RHITexture;
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

	virtual void Execute(RHIContext *context, CameraData* cameraData) override;

private:

	std::vector<RHIRenderTarget*> m_pRenderTargets;

	std::vector<RHITexture*> m_pDepthTextures;

	Mesh *m_pMesh;

	RHIBuffer *m_pVertexBuffer;

	RHIBuffer *m_pIndexBuffer;

	RHIBuffer* m_pUniformBuffer;

	RHIBuffer *m_pSurfaceDataBuffer;

	RHIPipelineResourceLayout *m_pPipelineResourceLayout;

	RHIGroup *m_pGroup;

};