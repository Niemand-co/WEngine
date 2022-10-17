#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class Mesh;
class RHIRenderTarget;
class RHIBuffer;
class RHIGroup;
class RHIContext;
class RHICommandBuffer;

struct CameraData;

class DrawSkyboxPass : public ScriptableRenderPass
{
public:

	DrawSkyboxPass(RenderPassConfigure* configure);

	virtual ~DrawSkyboxPass();

	virtual void Setup(RHIContext *context, CameraData *cameraData) override;

	virtual void Execute(RHIContext *context, CameraData* cameraData) override;

private:

	std::vector<RHIRenderTarget*> m_pRenderTargets;

	std::vector<RHICommandBuffer*> m_pCommandBuffers;

	Mesh *m_pMesh;

	RHIGroup *m_pGroup;

	RHIPipelineResourceLayout *m_pPipelineLayout;

	RHIBuffer *m_pVertexBuffer;

	RHIBuffer *m_pIndexBuffer;

	RHIBuffer *m_pUniformBuffer;

};