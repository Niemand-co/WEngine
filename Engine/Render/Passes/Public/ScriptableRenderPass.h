#pragma once

class RHIDevice;
class RHIContext;
class RHIPipelineStateObject;
class RHIRenderPass;
class RHIGroup;
class RHISemaphore;
class RHIFence;
struct CameraData;

struct RenderPassConfigure
{
	RHIDevice *pDevice;
	RHIRenderPass *pRenderPass;
	RHIPipelineStateObject *pPSO;
};

class ScriptableRenderPass
{
public:

	ScriptableRenderPass(RenderPassConfigure *configure);

	virtual ~ScriptableRenderPass();

	virtual void Setup(RHIContext *context, CameraData *cameraData) = 0;

	virtual void Execute(RHIContext *context, RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore, RHIFence *fence = nullptr) = 0;

	virtual RHIGroup* GetResourceGroup();

protected:

	RHIDevice *m_pDevice;

	RHIRenderPass *m_pRenderPass;

	RHIGroup *m_pResource;

	RHIPipelineStateObject *m_pPSO;

};
