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

	virtual int GetNextImage(RHISemaphore* pSignalSemaphore);

	virtual void Submit(RHISubmitDescriptor* descriptor);

	virtual int32 AcquireImageIndex(RHISemaphore** OutSemaphore);

	virtual void Present(unsigned int imageIndex);

	virtual bool IsDisplayChanged();

	virtual void ResetDisplayState();

	WVertexBufferRHIRef CreateVertexBuffer(size_t stride, size_t count);

	WIndexBufferRHIRef CreateIndexBuffer(size_t count);

	WUniformBufferRHIRef CreateUniformBuffer(size_t stride, size_t count);

	WDynamicUniformBufferRHIRef* CreateDynamicUniformBuffer(size_t stride, size_t count);

	RHIBuffer* CreateTextureBuffer(RHIBufferDescriptor *descriptor);

	WVertexShaderRHIRef CreateVertexShader(ShaderCodeBlob& blob);

	WPixelShaderRHIRef CreatePixelShader(ShaderCodeBlob& blob);

	WGeometryShaderRHIRef CreateGeometryShader(ShaderCodeBlob& blob);

	WComputeShaderRHIRef CreateComputeShader(ShaderCodeBlob& blob);

	WTexture2DRHIRef CreateTexture2D(uint32 InWidth, uint32 InHeight, Format InFormat, uint32 InMipCount, ETextureCreateFlags InFlag);

	WTexture2DArrayRHIRef CreateTexture2DArray(uint32 InWidth, uint32 InHeight, Format InFormat, uint32 InMipCount, uint32 InLayerCount, ETextureCreateFlags InFlag);

	WTexture3DRHIRef CreateTexture3D(uint32 InWidth, uint32 InHeight, uint32 InDepth, Format InFormat, uint32 InMipCount, ETextureCreateFlags InFlag);

	WTextureViewRHIRef CreateTextureView(uint32 InMipIndex, uint32 InMipCount, uint32 InLayerIndex, uint32 InLayerCount, uint32 InPlaneIndex, uint32 InPlaneCount, Dimension InDimension, Format InFormat, RHITexture* InTexture);

	WRenderPassRHIRef CreateRenderPass(class RHIRenderPassDescriptor *descriptor);

	virtual RHIScissor* CreateScissor(RHIScissorDescriptor *descriptor);

	virtual RHIViewport* CreateViewport(RHIViewportDescriptor *descriptor);

	virtual void CopyBufferToImage(RHITexture *pTexture, RHIBuffer *pBuffer, unsigned int width, unsigned int height);

	virtual RHIGroupLayout* CreateGroupLayout(RHIGroupLayoutDescriptor *descriptor);

	virtual WEngine::WArray<RHIGroup*> CreateResourceGroup(RHIGroupDescriptor *descriptor);

	virtual void UpdateUniformResourceToGroup(RHIUpdateResourceDescriptor *descriptor);

	virtual void UpdateTextureResourceToGroup(RHIUpdateResourceDescriptor *descriptor);

	virtual RHIPipelineResourceLayout* CreatePipelineResourceLayout(RHIPipelineResourceLayoutDescriptor *descriptor);

	virtual RHIPipelineStateObject* CreatePSO(RHIPipelineStateObjectDescriptor *descriptor);

	virtual void RHIBeginDrawingViewport(class RHIViewport* Viewport) = 0;

	virtual void RHIEndDrawingViewport(class RHIViewport* Viewport) = 0;

public:

	static inline RHIContext* GetContext() { return g_pContext; }

	static inline RHIInstance* GetInstance() { return g_pInstance; }

	static inline RHIGPU* GetGPU() { return g_pInstance->GetGPU(0); }

	static inline RHIDevice* GetDevice() { return g_pDevice; }

	static inline RHIQueue* GetQueue() { return g_pQueue; }

	static inline void Wait() { g_pDevice->Wait(); }

	template<typename T>
	static T* CreateRenderPipeline();

private:

	static RHIInstance* g_pInstance;

	static RHIDevice* g_pDevice;

	static RHISwapchain* g_pSwapchain;

	static RHIContext* g_pContext;

	static RHIQueue *g_pQueue;

	static bool m_isDisplayChagned;

};

template<typename T>
inline T* RHIContext::CreateRenderPipeline()
{
	return new T(g_pContext);
}
