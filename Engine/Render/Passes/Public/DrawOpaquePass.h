#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHITexture;
class RHISampler;
class Mesh;
class RHIBuffer;
class RHIGroup;
class RHIPipelineResourceLayout;

class DrawOpaquePass : public ScriptableRenderPass
{
public:

	DrawOpaquePass();

	virtual ~DrawOpaquePass();

	virtual void Setup(RHIContext *context, CameraData *cameraData) override;

	virtual void Execute(RHIContext *context, CameraData* cameraData) override;

	virtual void UpdateRenderTarget(CameraData *cameraData) override;

private:

	RHITexture *m_pTexture;

	RHISampler *m_pSampler;

	Mesh *m_pMesh;

	RHIBuffer* m_pVertexBuffer;

	RHIBuffer* m_pIndexBuffer;

	RHIBuffer* m_pUniformBuffer;

	RHIBuffer *m_pSurfaceDataBuffer;

	RHIPipelineResourceLayout *m_pPipelineResourceLayout;

	RHIGroup *m_pGroup;

};