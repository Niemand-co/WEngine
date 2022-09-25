#pragma once

class RHIDevice;
class RHIContext;
class RHIPipelineStateObject;
class RHIRenderPass;
class RHISemaphore;

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

	virtual void Setup();

	virtual void Execute(RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore);

private:

	RHIDevice *m_pDevice;

	RHIContext *m_pContext;

	RHIRenderPass *m_pRenderPass;

	RHIPipelineStateObject *m_pPSO;

};
