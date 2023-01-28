#pragma once

class RHIDevice;
class RHIContext;
class RHIPipelineStateObject;
class RHIRenderPass;
class RHIGroup;
class RHICommandBuffer;
class RHISemaphore;
class RHIRenderTarget;
class ScriptableRenderer;
struct CameraData;

class ScriptableRenderPass
{
public:

	ScriptableRenderPass(ScriptableRenderer* pRenderer);

	virtual ~ScriptableRenderPass();

	virtual void Setup(RHIContext* context, CameraData *cameraData) = 0;

	virtual void Execute(RHIContext* context, CameraData* cameraData) = 0;

	virtual void UpdateRenderTarget(CameraData *cameraData) = 0;

	virtual RHIGroup* GetResourceGroup();

	void* operator new(size_t size)
	{
		return WEngine::Allocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		WEngine::Allocator::Get()->Deallocate(pData);
	}

protected:

	WEngine::WArray<RHIRenderTarget*> m_pRenderTargets;

	RHIDevice *m_pDevice;

	RHIRenderPass *m_pRenderPass;

	RHIGroup *m_pResource;

	RHIPipelineStateObject *m_pPSO;

	WEngine::WArray<RHICommandBuffer*> m_pCommandBuffers;

	ScriptableRenderer *m_pRenderer;

};
