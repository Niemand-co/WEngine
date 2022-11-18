#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class Mesh;
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

	DrawSkyboxPass();

	virtual ~DrawSkyboxPass();

	virtual void Setup(RHIContext *context, CameraData *cameraData) override;

	virtual void Execute(RHIContext *context, CameraData* cameraData) override;

	virtual void UpdateRenderTarget(CameraData *cameraData) override;

private:

	std::vector<RHICommandBuffer*> m_pCommandBuffers;

	std::vector<RHITexture*> m_pCubemap;

	Mesh *m_pMesh;

	std::vector<RHIGroup*> m_pGroup;

	RHIPipelineResourceLayout *m_pPipelineLayout;

	RHIBuffer *m_pUniformBuffer;

public:

	static glm::vec4 topColor;

	static glm::vec4 bottomColor;

};