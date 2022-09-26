#pragma once

class ScriptableRenderPass;

class RHIDevice;
class RHIContext;
class RHISemaphore;

struct RendererConfigure
{
	RHIDevice *pDevice;
	RHIContext *pContext;
};

class ScriptableRenderer
{
public:

	ScriptableRenderer(RendererConfigure *pConfigure);

	virtual ~ScriptableRenderer();

	virtual void Setup();

	virtual void Execute(RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore);

	virtual void AddRenderPass();

private:

	std::vector<ScriptableRenderPass*> m_passes;

	RHIDevice *m_pDevice;

	RHIContext *m_pContext;

	ScriptableRenderPass *m_drawOpaquePass;

	ScriptableRenderPass *m_mainLightShadowPass;

	ScriptableRenderPass *m_finalBlitPass;

};