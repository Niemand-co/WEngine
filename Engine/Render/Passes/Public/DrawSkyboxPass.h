#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class WStaticMesh;
class RHIBuffer;
class RHITexture;
class RHIGroup;
class RHIContext;
class RHICommandBuffer;
class RHIPipelineResourceLayout;

struct CameraInfo;

class DrawSkyboxPass : public ScriptableRenderPass
{
public:

	DrawSkyboxPass(ScriptableRenderer* pRenderer);

	virtual ~DrawSkyboxPass();

	virtual void Setup() override;

	virtual void Execute() override;

private:

	WEngine::WArray<RHICommandBuffer*> m_pCommandBuffers;

	WEngine::WArray<RHITexture*> m_pCubemap;

	WStaticMesh *m_pMesh;

	WEngine::WArray<RHIGroup*> m_pGroup;

	RHIPipelineResourceLayout *m_pPipelineLayout;

	RHIBuffer *m_pUniformBuffer;

public:

	static glm::vec4 topColor;

	static glm::vec4 bottomColor;

};