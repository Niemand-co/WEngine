#pragma once

class ScriptableRenderPass;

class RHIDevice;
class RHIContext;
class RHISemaphore;
class RHIFence;
class RHIEvent;
class RHITextureView;
struct CameraData;

class ScriptableRenderer
{
public:

	ScriptableRenderer(RHIContext *pContext);

	virtual ~ScriptableRenderer();

	virtual void Setup(RHIContext* context, CameraData *cameraData);

	virtual void Execute(RHIContext *context, CameraData* cameraData);

	virtual void EnqueRenderPass(ScriptableRenderPass* renderPass);

	virtual void UpdateRenderTarget(CameraData* cameraData);

	void SetGlobalTexture(std::vector<RHITextureView*>& pTexture) { m_pGlobalTextures.push_back(pTexture); }

	const std::vector<std::vector<RHITextureView*>>& GetGlobalTextures() const { return m_pGlobalTextures; }

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

	RHIContext *m_pContext;

	RHIEvent * m_pEvent;

	std::vector<RHISemaphore*> m_pWaitSemaphore;

	std::vector<RHISemaphore*> m_pSignalSemaphore;

	std::vector<std::string_view> m_blockSubmission;

	std::vector<std::vector<RHITextureView*>> m_pGlobalTextures;

};