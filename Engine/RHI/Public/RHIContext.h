#pragma once
#include "Render/Public/RenderContext.h"
#include "RHI/Public/RHIInstance.h"
#include "RHI/Public/RHIDevice.h"

class RHIDevice;
class RHIGPU;
class RHIQueue;
class RHISwapchain;
class RHISurface;
class RHITexture;
class RHITextureView;
class RHICommandPool;
class RHICommandBuffer;
class RHIBuffer;
class RHIGroup;
class RHIGroupLayout;
class RHIPipelineResourceLayout;
class RHIPipelineStateObject;
class RHISemaphore;
class RHIFence;
class RHIScissor;
class RHIViewport;

class RHIBufferDescriptor;
class RHIScissorDescriptor;
class RHIViewportDescriptor;
class RHIGroupLayoutDescriptor;
class RHIGroupDescriptor;
class RHIUpdateResourceDescriptor;
class RHIPipelineResourceLayoutDescriptor;
class RHIPipelineStateObjectDescriptor;
class RHISubmitDescriptor;
class RHIBarrierDescriptor;

class RHIContext : public RenderContext
{
public:

	RHIContext();

	virtual ~RHIContext() = default;

	static void Init();

	virtual void RecreateSwapchain();

	virtual RHITexture* GetTexture(unsigned int index);

	virtual RHITextureView* GetTextureView(unsigned int index);

	virtual RHITextureView* GetDepthView(unsigned int index);

	virtual RHICommandBuffer* GetCommandBuffer(bool isPrimary = true);

	virtual WEngine::WArray<RHICommandBuffer*> GetCommandBuffer(unsigned int count, bool isPrimary);

	virtual int GetNextImage(RHISemaphore* pSignalSemaphore);

	virtual void ExecuteCommandBuffer(RHICommandBuffer *cmd);

	virtual void Submit(RHISubmitDescriptor *descriptor);

	virtual int GetNextImage();

	virtual RHISemaphore* GetImageVailableSemaphore() { return g_pImageAvailibleSemaphores[g_currentFrame]; }

	virtual RHISemaphore* GetPresentVailableSemaphore() { return g_pPresentAVailibleSemaphores[g_currentFrame]; }

	virtual RHIFence* GetFence() { return g_pFences[g_currentFrame]; }

	virtual void Present(unsigned int imageIndex);

	virtual bool IsDisplayChanged();

	virtual void ResetDisplayState();

	virtual void ResourceBarrier(RHIBarrierDescriptor* pDescriptor);

	virtual WVertexBuffer* CreateVertexBuffer(size_t stride, size_t count);

	virtual Buffer* CreateIndexBuffer(size_t count);

	virtual Buffer* CreateUniformBuffer(size_t stride, size_t count);

	virtual RHIBuffer* CreateTextureBuffer(RHIBufferDescriptor *descriptor);

	virtual RHIScissor* CreateScissor(RHIScissorDescriptor *descriptor);

	virtual RHIViewport* CreateViewport(RHIViewportDescriptor *descriptor);

	virtual void CopyBufferToImage(RHITexture *pTexture, RHIBuffer *pBuffer, unsigned int width, unsigned int height);

	virtual RHIGroupLayout* CreateGroupLayout(RHIGroupLayoutDescriptor *descriptor);

	virtual WEngine::WArray<RHIGroup*> CreateResourceGroup(RHIGroupDescriptor *descriptor);

	virtual void UpdateUniformResourceToGroup(RHIUpdateResourceDescriptor *descriptor);

	virtual void UpdateTextureResourceToGroup(RHIUpdateResourceDescriptor *descriptor);

	virtual RHIPipelineResourceLayout* CreatePipelineResourceLayout(RHIPipelineResourceLayoutDescriptor *descriptor);

	virtual RHIPipelineStateObject* CreatePSO(RHIPipelineStateObjectDescriptor *descriptor);

public:

	static inline RHIContext* GetContext() { return g_pContext; }

	static inline RHIInstance* GetInstance() { return g_pInstance; }

	static inline RHIGPU* GetGPU() { return g_pInstance->GetGPU(0); }

	static inline RHIDevice* GetDevice() { return g_pDevice; }

	static inline RHIQueue* GetQueue() { return g_pQueue; }

	static inline void Wait() { g_pDevice->Wait(); }

	template<typename T>
	static T* CreateRenderPipeline();

public:

	static unsigned int g_maxFrames;

	static unsigned int g_currentFrame;

	static int g_currentImage;

private:

	static RHIInstance* g_pInstance;

	static RHIDevice* g_pDevice;

	static RHISwapchain* g_pSwapchain;

	static RHIContext* g_pContext;

	static RHIQueue *g_pQueue;

	static RHISurface *g_pSurface;

	static RHICommandPool *g_pPool;

	static WEngine::WArray<RHISemaphore*> g_pImageAvailibleSemaphores;

	static WEngine::WArray<RHISemaphore*> g_pPresentAVailibleSemaphores;

	static WEngine::WArray<RHIFence*> g_pFences;

	static WEngine::WArray<RHITextureView*> g_pTextureViews;

	static WEngine::WArray<RHITexture*> g_pDepthTextures;

	static WEngine::WArray<RHITextureView*> g_pDepthTextureViews;

	static WEngine::WArray<RHICommandBuffer*> g_pCommandBuffers;

	static WEngine::WArray<RHICommandBuffer*> g_pPrimaryCommandBuffers;

	static bool m_isDisplayChagned;

};

template<typename T>
inline T* RHIContext::CreateRenderPipeline()
{
	return new T(g_pContext);
}
