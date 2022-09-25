#pragma once

class ScriptableRenderer;

class RHIInstance;
class RHIGPU;
class RHIDevice;
class RHISwapchain;
class RHIContext;
class RHISemaphore;
class RHITexture;

class ScriptableRenderPipeline
{
public:

	ScriptableRenderPipeline();

	virtual ~ScriptableRenderPipeline();

	static ScriptableRenderPipeline* get();

	virtual void Init();

	virtual void Setup();

	virtual void Execute();

	virtual void AddRenderer();

	virtual void Present();

public:

	static int g_currentFrame;

	static RHITexture* g_currentRenderTarget;

private:

	static ScriptableRenderPipeline *g_instance;

private:

	std::vector<ScriptableRenderer*> m_renderers;

	RHIInstance *m_pInstance;

	RHIDevice *m_pDevice;

	RHISwapchain *m_pSwapchain;

	RHIContext *m_pContext;

	std::vector<RHISemaphore*> m_semaphores;

	unsigned int m_semaphoreCount;

};