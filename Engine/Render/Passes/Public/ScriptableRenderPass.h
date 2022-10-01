#pragma once

class RHIDevice;
class RHIContext;
class RHIPipelineStateObject;
class RHIRenderPass;
class RHISemaphore;
class RHIFence;

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

	virtual void Setup(RHIContext *context) = 0;

	virtual void Execute(RHIContext *context, RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore, RHIFence *fence = nullptr) = 0;

protected:

	RHIDevice *m_pDevice;

	RHIRenderPass *m_pRenderPass;

	RHIPipelineStateObject *m_pPSO;

};
