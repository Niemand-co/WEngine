#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHITexture;
class RHISampler;
class WStaticMesh;
class RHIBuffer;
class RHIGroup;
class RHIPipelineResourceLayout;

class DrawOpaquePass : public ScriptableRenderPass
{
public:

	DrawOpaquePass(ScriptableRenderer* pRenderer);

	virtual ~DrawOpaquePass();

	virtual void Setup() override;

	virtual void Execute() override;

private:

	RHISampler *m_pSampler;

	WEngine::WArray<RHIBuffer*> m_pObjectUniformBuffers;

	WEngine::WArray<RHIBuffer*> m_pSceneUniformBuffers;

	RHIBuffer *m_pSurfaceDataBuffer;

	RHIPipelineResourceLayout *m_pPipelineResourceLayout;

	WEngine::WArray<RHIGroup*> m_pGroup;

};