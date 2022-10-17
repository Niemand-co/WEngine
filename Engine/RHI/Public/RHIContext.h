#pragma once
#include "Render/Public/RenderContext.h"

class RHIDevice;
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

class RHIBufferDescriptor;
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

	RHIContext(RHIQueue *queue, RHISurface *surface, RHIDevice *device);

	virtual ~RHIContext() = default;

	void Init();

	virtual void RecreateSwapchain();

	virtual RHITexture* GetTexture(unsigned int index);

	virtual RHITextureView* GetTextureView(unsigned int index);

	virtual RHITextureView* GetDepthView(unsigned int index);

	virtual RHICommandBuffer* GetCommandBuffer(bool isPrimary = true);

	virtual std::vector<RHICommandBuffer*> GetCommandBuffer(unsigned int count, bool isPrimary);

	virtual int GetNextImage(RHISemaphore* pSignalSemaphore);

	virtual void ExecuteCommandBuffer(RHICommandBuffer *cmd);

	virtual void Submit(RHISubmitDescriptor *descriptor);

	virtual bool Present(unsigned int imageIndex, RHISemaphore *semaphore);

	virtual bool IsDisplayChanged();

	virtual void ResetDisplayState();

	virtual void ImageLayoutTransition(RHIBarrierDescriptor* pDescriptor);

	virtual RHIBuffer* CreateVertexBuffer(RHIBufferDescriptor *descriptor);

	virtual RHIBuffer* CreateIndexBuffer(RHIBufferDescriptor *descriptor);

	virtual RHIBuffer* CreateUniformBuffer(RHIBufferDescriptor *descriptor);

	virtual RHIGroupLayout* CreateGroupLayout(RHIGroupLayoutDescriptor *descriptor);

	virtual RHIGroup* CreateResourceGroup(RHIGroupDescriptor *descriptor);

	virtual void UpdateResourceToGroup(RHIUpdateResourceDescriptor *descriptor);

	virtual RHIPipelineResourceLayout* CreatePipelineResourceLayout(RHIPipelineResourceLayoutDescriptor *descriptor);

	virtual RHIPipelineStateObject* CreatePSO(RHIPipelineStateObjectDescriptor *descriptor);

protected:

	RHIDevice *m_pDevice;

	RHIQueue *m_pQueue;

	RHISurface *m_pSurface;

	RHISwapchain *m_pSwapchain;

	RHICommandPool *m_pPool;

	std::vector<RHITextureView*> m_pTextureViews;

	std::vector<RHITexture*> m_pDepthTextures;

	std::vector<RHITextureView*> m_pDepthTextureViews;

	std::vector<RHICommandBuffer*> m_pCommandBuffers;

	std::vector<RHICommandBuffer*> m_pPrimaryCommandBuffers;

	bool m_isDisplayChagned;

};