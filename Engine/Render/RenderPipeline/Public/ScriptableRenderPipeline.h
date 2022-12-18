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
	friend class RHIContext;

public:

	ScriptableRenderPipeline(RHIContext *context);

	virtual ~ScriptableRenderPipeline();

	virtual void Init();

	virtual void Setup();

	virtual void Execute();

	static ScriptableRenderer* CreateRenderer();

	//virtual void RenderSingleCamera(Camera *camera, RHIContext* context);

protected:

	void* operator new(size_t size)
	{
		return WEngine::Allocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		WEngine::Allocator::Get()->Deallocate(pData);
	}

protected:

	RHIContext *m_pContext;

	WEngine::WArray<Camera*> m_pCameras;

	WEngine::WArray<CameraData*> m_pCameraDatas;

};