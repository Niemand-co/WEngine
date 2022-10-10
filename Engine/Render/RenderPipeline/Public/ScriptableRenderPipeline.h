#pragma once

class ScriptableRenderer;

class RHIInstance;
class RHIGPU;
class RHIDevice;
class RHISwapchain;
class RHIContext;
class RHISemaphore;
class RHIFence;
class RHITexture;
class Camera;
struct CameraData;

class ScriptableRenderPipeline
{
public:

	ScriptableRenderPipeline();

	virtual ~ScriptableRenderPipeline();

	static ScriptableRenderPipeline* get();

	virtual void Init();

	virtual void Setup();

	virtual void Execute();

	virtual ScriptableRenderer* CreateRenderer();

	virtual void RenderSingleCamera(Camera *camera, RHIContext* context, RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence* fence);

public:

	static int g_currentImage;

	static unsigned int g_currentFrame;

private:

	static ScriptableRenderPipeline *g_instance;

private:

	RHIInstance *m_pInstance;

	RHIDevice *m_pDevice;

	RHISwapchain *m_pSwapchain;

	RHIContext *m_pContext;

	std::vector<RHISemaphore*> m_pImageAvailibleSemaphores;

	std::vector<RHISemaphore*> m_pPresentAVailibleSemaphores;

	std::vector<RHIFence*> m_pFences;

	unsigned int m_maxFrame;

	std::vector<Camera*> m_pCameras;

	std::vector<CameraData*> m_pCameraDatas;

};