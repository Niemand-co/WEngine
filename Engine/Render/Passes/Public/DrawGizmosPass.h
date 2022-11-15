#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHIGroup;
class RHIBuffer;
class RHIPipelineResourceLayout;

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

};