#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHIGroup;
class RHIBuffer;
class RHIPipelineResourceLayout;
class RHIBuffer;
class StaticMesh;
class RHITexture;
class RHITextureView;

class DrawGizmosPass : public ScriptableRenderPass
{
public:

	DrawGizmosPass(ScriptableRenderer* pRenderer);

	~DrawGizmosPass();

	virtual void Setup(RHIContext* context, CameraData* cameraData) override;

	virtual void Execute(RHIContext* context, CameraData* cameraData) override;

	virtual void UpdateRenderTarget(CameraData* cameraData) override;

private:

	WEngine::WArray<RHIGroup*> m_pGroup;

	RHIBuffer* m_pSceneBuffer;

	RHIBuffer *m_pObjectBuffer;

	RHIPipelineResourceLayout *m_pResourceLayout;

	RHIRenderPass* m_pStencilRenderPass;

	RHIPipelineStateObject *m_pStencilPSO;

	RHIPipelineStateObject* m_pDebugPSO;

	RHIPipelineStateObject* m_pDebugLinePSO;

	WEngine::WArray<RHITexture*> m_pStencils;

	WEngine::WArray<RHITextureView*> m_pStencilViews;

	WEngine::WArray<RHIRenderTarget*> m_pStencilRenderTargets;

	StaticMesh *m_pAxisMesh;

};