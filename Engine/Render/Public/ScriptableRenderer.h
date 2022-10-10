#pragma once

class ScriptableRenderPass;

class RHIDevice;
class RHIContext;
class RHISemaphore;
class RHIFence;
struct CameraData;

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

	virtual void Setup(CameraData *cameraData);

	virtual void Execute(RHIContext *context, RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore, RHIFence *fence);

	virtual void AddRenderPass();

private:

	std::vector<ScriptableRenderPass*> m_passes;

	RHIDevice *m_pDevice;

	RHIContext *m_pContext;

	ScriptableRenderPass *m_drawOpaquePass;

	ScriptableRenderPass *m_mainLightShadowPass;

	ScriptableRenderPass *m_finalBlitPass;

	ScriptableRenderPass *m_drawGuiPass;

	std::vector<RHISemaphore*> m_semaphores;

};