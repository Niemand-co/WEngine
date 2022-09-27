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
	RHIContext *pContext;
	RHIRenderPass *pRenderPass;
	RHIPipelineStateObject *pPSO;
};

class ScriptableRenderPass
{
public:

	ScriptableRenderPass(RenderPassConfigure *configure);

	virtual ~ScriptableRenderPass();

	virtual void Setup() = 0;

	virtual void Execute(RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore, RHIFence *fence = nullptr) = 0;

protected:

	RHIDevice *m_pDevice;

	RHIContext *m_pContext;

	RHIRenderPass *m_pRenderPass;

	RHIPipelineStateObject *m_pPSO;

};
