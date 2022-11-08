#pragma once

class RHIDevice;
class RHIContext;
class RHIPipelineStateObject;
class RHIRenderPass;
class RHIGroup;
class RHISemaphore;
class RHIFence;
class RHIEvent;
class RHICommandBuffer;
struct CameraData;

class ScriptableRenderPass
{
public:

	ScriptableRenderPass();

	virtual ~ScriptableRenderPass();

	virtual void Setup(RHIContext *context, CameraData *cameraData) = 0;

	virtual void Execute(RHIContext *context, CameraData* cameraData) = 0;

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

	RHIDevice *m_pDevice;

	RHIRenderPass *m_pRenderPass;

	RHIGroup *m_pResource;

	RHIPipelineStateObject *m_pPSO;

	std::vector<RHICommandBuffer*> m_pCommandBuffers;

};
