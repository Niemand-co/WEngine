#pragma once
#include "Render/Public/Scene.h"
#include "Scene/Public/SceneManagement.h"

class ScriptableRenderPass;

class RHIDevice;
class RHIContext;
class RHISemaphore;
class RHIFence;
class RHIEvent;
class RHITextureView;
struct CameraInfo;

class ScriptableRenderer
{
public:

	ScriptableRenderer(class CameraComponent * pCamera, class WViewport* Viewport);

	virtual ~ScriptableRenderer();

	virtual void Render() = 0;

	void* operator new(size_t size)
	{
		return NormalAllocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		NormalAllocator::Get()->Deallocate(pData);
	}

protected:

	WEngine::WArray<ScriptableRenderPass*> m_passes;

	RHIContext *m_pContext;

	RHIEvent *m_pEvent;

	WEngine::WArray<RHISemaphore*> m_pWaitSemaphore;

	WEngine::WArray<RHISemaphore*> m_pSignalSemaphore;

	CameraComponent *m_pCamera;

	WViewport *Viewport;

};

struct SceneViewInfo
{
	uint8 Visibility : 1;
};

class SceneRenderer : public ScriptableRenderer
{
public:

	SceneRenderer(CameraComponent* pCamera, WViewport* Viewport)
		: ScriptableRenderer(pCamera, Viewport)
	{
	}

	virtual ~SceneRenderer() = default;

	void SetScene(RScene *inScene) { Scene = inScene; }

protected:

	RScene *Scene;

	WEngine::WArray<SceneViewInfo> Views;

	WEngine::WArray<WMeshBatch*> Batches;

	WMeshCollector Collector;

};