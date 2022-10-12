#pragma once

class RHIDevice;
class RHIContext;
class RHIPipelineStateObject;
class RHIRenderPass;
class RHIGroup;
class RHISemaphore;
class RHIFence;
class RHIEvent;
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

	virtual void Execute(RHIContext *context, RHISemaphore *waitSemaphore = nullptr, RHISemaphore *signalSemaphore = nullptr, RHIFence *fence = nullptr, RHIEvent *pEvent = nullptr) = 0;

	virtual RHIGroup* GetResourceGroup();

protected:

	RHIDevice *m_pDevice;

	RHIRenderPass *m_pRenderPass;

	RHIGroup *m_pResource;

	RHIPipelineStateObject *m_pPSO;

};
