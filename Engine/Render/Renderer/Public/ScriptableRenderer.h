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

	ScriptableRenderer();

	virtual ~ScriptableRenderer();

	virtual void Setup(RHIContext* context, CameraInfo *cameraData);

	virtual void Execute(RHIContext *context, CameraInfo* cameraData);

	virtual void EnqueRenderPass(ScriptableRenderPass* renderPass);

	virtual void UpdateRenderTarget(CameraInfo* cameraData);

	void SetGlobalTexture(WEngine::WArray<RHITextureView*>& pTexture) { m_pGlobalTextures.Push(pTexture); }

	const WEngine::WArray<WEngine::WArray<RHITextureView*>>& GetGlobalTextures() const { return m_pGlobalTextures; }

	void* operator new(size_t size)
	{
		return WEngine::Allocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		WEngine::Allocator::Get()->Deallocate(pData);
	}

private:

	WEngine::WArray<ScriptableRenderPass*> m_passes;

	RHIContext *m_pContext;

	RHIEvent *m_pEvent;

	WEngine::WArray<RHISemaphore*> m_pWaitSemaphore;

	WEngine::WArray<RHISemaphore*> m_pSignalSemaphore;

	WEngine::WArray<WEngine::WString> m_blockSubmission;

	WEngine::WArray<WEngine::WArray<RHITextureView*>> m_pGlobalTextures;

};

class SceneRenderer : public ScriptableRenderer
{
public:

	virtual ~SceneRenderer() = default;

protected:

	RScene *Scene;

	WEngine::WArray<WMeshBatch*> Batches;

};