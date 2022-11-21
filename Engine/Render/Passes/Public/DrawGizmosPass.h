#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHIGroup;
class RHIBuffer;
class RHIPipelineResourceLayout;
class RHIBuffer;
class Mesh;
class RHITexture;
class RHITextureView;

class DrawGizmosPass : public ScriptableRenderPass
{
public:

	DrawGizmosPass();

	~DrawGizmosPass();

	virtual void Setup(RHIContext* context, CameraData* cameraData) override;

	virtual void Execute(RHIContext* context, CameraData* cameraData) override;

	virtual void UpdateRenderTarget(CameraData* cameraData) override;

private:

	std::vector<RHIGroup*> m_pGroup;

	RHIBuffer* m_pSceneBuffer;

	RHIBuffer *m_pObjectBuffer;

	RHIPipelineResourceLayout *m_pResourceLayout;

	RHIRenderPass* m_pStencilRenderPass;

	RHIPipelineStateObject *m_pStencilPSO;

	std::vector<RHITexture*> m_pStencils;

	std::vector<RHITextureView*> m_pStencilViews;

	std::vector<RHIRenderTarget*> m_pStencilRenderTargets;

};