#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class StaticMesh;
class RHIBuffer;
class RHITexture;
class RHIGroup;
class RHIContext;
class RHICommandBuffer;
class RHIPipelineResourceLayout;

struct CameraData;

class DrawSkyboxPass : public ScriptableRenderPass
{
public:

	DrawSkyboxPass(ScriptableRenderer* pRenderer);

	virtual ~DrawSkyboxPass();

	virtual void Setup(RHIContext *context, CameraData *cameraData) override;

	virtual void Execute(RHIContext *context, CameraData* cameraData) override;

	virtual void UpdateRenderTarget(CameraData *cameraData) override;

private:

	WEngine::WArray<RHICommandBuffer*> m_pCommandBuffers;

	WEngine::WArray<RHITexture*> m_pCubemap;

	StaticMesh *m_pMesh;

	WEngine::WArray<RHIGroup*> m_pGroup;

	RHIPipelineResourceLayout *m_pPipelineLayout;

	RHIBuffer *m_pUniformBuffer;

public:

	static glm::vec4 topColor;

	static glm::vec4 bottomColor;

};