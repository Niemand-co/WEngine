#pragma once

class ScriptableRenderPass;

class RHIDevice;
class RHIContext;
class RHISemaphore;
class RHIFence;
class RHIEvent;
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

	virtual void Setup(RHIContext* context, CameraData *cameraData);

	virtual void Execute(RHIContext *context, RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore, RHIFence *fence);

	virtual void EnqueRenderPass(ScriptableRenderPass* renderPass);

	void* operator new(size_t size)
	{
		return WEngine::Allocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		WEngine::Allocator::Get()->Deallocate(pData);
	}

private:

	std::vector<ScriptableRenderPass*> m_passes;

	RHIDevice *m_pDevice;

	std::vector<RHISemaphore*> m_semaphores;

	RHIEvent * m_pEvent;

};