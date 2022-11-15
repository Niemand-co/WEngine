#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHIGroup;
class RHIBuffer;
class RHIPipelineResourceLayout;
class RHIBuffer;
class Mesh;

class DrawGizmosPass : public ScriptableRenderPass
{
public:

	DrawGizmosPass();

	~DrawGizmosPass();

	virtual void Setup(RHIContext* context, CameraData* cameraData) override;

	virtual void Execute(RHIContext* context, CameraData* cameraData) override;

	virtual void UpdateRenderTarget(CameraData* cameraData) override;

private:

	RHIGroup *m_pGroup;

	RHIBuffer *m_pBuffer;

	RHIPipelineResourceLayout *m_pResourceLayout;

	RHIBuffer *m_pVertexBuffer;

	RHIBuffer *m_pIndexBuffer;

	Mesh *m_pMesh;

};