#pragma once
#include "Render/Public/Scene.h"
#include "Render/Mesh/Public/MeshBatch.h"

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

	ScriptableRenderer(class CameraComponent * pCamera);

	virtual ~ScriptableRenderer();

	virtual void Render() = 0;

	void* operator new(size_t size)
	{
		return WEngine::Allocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		WEngine::Allocator::Get()->Deallocate(pData);
	}

protected:

	WEngine::WArray<ScriptableRenderPass*> m_passes;

	RHIContext *m_pContext;

	RHIEvent *m_pEvent;

	WEngine::WArray<RHISemaphore*> m_pWaitSemaphore;

	WEngine::WArray<RHISemaphore*> m_pSignalSemaphore;

	CameraComponent *m_pCamera;

};

struct SceneViewInfo
{
	uint8 Visibility : 1;
};

class SceneRenderer : public ScriptableRenderer
{
public:

	SceneRenderer(CameraComponent* pCamera)
		: ScriptableRenderer(pCamera)
	{
	}

	virtual ~SceneRenderer() = default;

	void SetScene(RScene *inScene) { Scene = inScene; }

protected:

	RScene *Scene;

	WEngine::WArray<SceneViewInfo> Views;

	WEngine::WArray<WMeshBatch*> Batches;

};