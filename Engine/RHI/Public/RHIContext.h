#pragma once
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
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

class RHIContext
{
public:

	RHIContext();

	virtual ~RHIContext() = default;

	static void Init(RHIBackend backend);

	WVertexBufferRHIRef CreateVertexBuffer(size_t stride, size_t count);

	WIndexBufferRHIRef CreateIndexBuffer(size_t count);

	WUniformBufferRHIRef CreateUniformBuffer(size_t stride, size_t count);

	WDynamicUniformBufferRHIRef* CreateDynamicUniformBuffer(size_t stride, size_t count);

	RHIBuffer* CreateTextureBuffer(RHIBufferDescriptor *descriptor);

	WVertexShaderRHIRef CreateVertexShader(ShaderCodeBlob& blob);

	WPixelShaderRHIRef CreatePixelShader(ShaderCodeBlob& blob);

	WGeometryShaderRHIRef CreateGeometryShader(ShaderCodeBlob& blob);

	WComputeShaderRHIRef CreateComputeShader(ShaderCodeBlob& blob);

	WTexture2DRHIRef CreateTexture2D(uint32 InWidth, uint32 InHeight, Format InFormat, uint32 InMipCount, ETextureCreateFlags InFlag, EAccess InitState);

	WTexture2DArrayRHIRef CreateTexture2DArray(uint32 InWidth, uint32 InHeight, Format InFormat, uint32 InMipCount, uint32 InLayerCount, ETextureCreateFlags InFlag, EAccess InitState);

	WTexture3DRHIRef CreateTexture3D(uint32 InWidth, uint32 InHeight, uint32 InDepth, Format InFormat, uint32 InMipCount, ETextureCreateFlags InFlag, EAccess InitState);

	WTextureViewRHIRef CreateTextureView(uint32 InMipIndex, uint32 InMipCount, uint32 InLayerIndex, uint32 InLayerCount, uint32 InPlaneIndex, uint32 InPlaneCount, Dimension InDimension, Format InFormat, RHITexture* InTexture);

	virtual RHIScissor* CreateScissor(RHIScissorDescriptor *descriptor);

	virtual void CopyBufferToImage(RHITexture *pTexture, RHIBuffer *pBuffer, unsigned int width, unsigned int height);

	virtual RHIGroupLayout* CreateGroupLayout(RHIGroupLayoutDescriptor *descriptor);

	virtual WEngine::WArray<RHIGroup*> CreateResourceGroup(RHIGroupDescriptor *descriptor);

	virtual void UpdateUniformResourceToGroup(RHIUpdateResourceDescriptor *descriptor);

	virtual void UpdateTextureResourceToGroup(RHIUpdateResourceDescriptor *descriptor);

	virtual RHIPipelineResourceLayout* CreatePipelineResourceLayout(RHIPipelineResourceLayoutDescriptor *descriptor);

	virtual RHIPipelineStateObject* CreatePSO(RHIPipelineStateObjectDescriptor *descriptor);

	virtual WViewportRHIRef CreateViewport(uint32 InWidth, uint32 InHeight, bool bInFullScreen, Format InFormat);

	virtual void RHIBeginDrawingViewport(class RHIViewport* Viewport) = 0;

	virtual void RHIEndDrawingViewport(class RHIViewport* Viewport, bool bPresent) = 0;

	virtual void RHIBeginRenderPass(class RHIRenderPassDescriptor* RenderPasDescriptor, class RHIFramebufferDescriptor* FramebufferDescriptor) = 0;

	virtual void RHIEndRenderPass() = 0;

	virtual void RHIBeginTransition(WEngine::WArray<class RHIBarrierDescriptor>& Transitions) = 0;

	virtual void CopyImageToBackBuffer(RHITexture* SrcTexture, RHITexture* DstTexture, int32 SrcSizeX, int32 SrcSizeY, int32 DstSizeX, int32 DstSizeY) = 0;

public:

	static inline RHIContext* GetContext() { return g_pContext; }

public:

	inline RHIInstance* GetInstance() { return pInstance; }

	inline RHIGPU* GetGPU() { return pInstance->GetGPU(0); }

	inline RHIDevice* GetDevice() { return pDevice; }

	inline RHIQueue* GetQueue() { return pQueue; }

	template<typename T>
	static T* CreateRenderPipeline();

protected:

	RHIInstance* pInstance;

	RHIDevice* pDevice;

	RHIQueue *pQueue;

private:

	static RHIContext* g_pContext;

	

};

template<typename T>
inline T* RHIContext::CreateRenderPipeline()
{
	return new T(g_pContext);
}
