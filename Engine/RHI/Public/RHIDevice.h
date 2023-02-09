#pragma once

class RHIQueue;
class RHISwapchain;
class RHIFence;
class RHIShader;
class RHIRenderPass;
class RHIPipelineStateObject;
class RHITexture;
class RHISampler;
class RHIRenderTarget;
class RHISemaphore;
class RHIFence;
class RHIEvent;
class RHIBuffer;
class RHIGroup;
class RHIGroupLayout;
class RHIGroupPool;
class RHIPipelineResourceLayout;
class RHIScissor;
class RHIViewport;

class RHISwapchainDescriptor;
class RHIShaderDescriptor;
class RHIRenderPassDescriptor;
class RHIPipelineStateObjectDescriptor;
class RHITextureDescriptor;
class RHISamplerDescriptor;
class RHIRenderTargetDescriptor;
class RHIBufferDescriptor;
class RHIGroupPoolDescriptor;
class RHIGroupDescriptor;
class RHIGroupLayoutDescriptor;
class RHIPipelineResourceLayoutDescriptor;
class RHIUpdateResourceDescriptor;
class RHIScissorDescriptor;
class RHIViewportDescriptor;

class RHIDevice
{
public:

	virtual ~RHIDevice() = default;

	virtual unsigned int GetQueueCount(RHIQueueType) = 0;

	virtual RHIQueue* GetQueue(RHIQueueType, unsigned int index) = 0;

	virtual RHISwapchain* CreateSwapchain(RHISwapchainDescriptor *descriptor) = 0;

	virtual WEngine::WArray<RHIFence*> CreateFence(unsigned int count) = 0;

	virtual RHIEvent* GetEvent() = 0;

	virtual WVertexShaderRHIRef CreateVertexShader(RHIShaderDescriptor* descriptor) = 0;

	virtual WPixelShaderRHIRef CreatePixelShader(RHIShaderDescriptor* descriptor) = 0;

	virtual WGeometryShaderRHIRef CreateGeometryShader(RHIShaderDescriptor* descriptor) = 0;

	virtual WComputeShaderRHIRef CreateComputeShader(RHIShaderDescriptor* descriptor) = 0;

	virtual RHIRenderPass* CreateRenderPass(RHIRenderPassDescriptor *descriptor) = 0;

	virtual RHIPipelineStateObject* CreatePipelineStateObject(RHIPipelineStateObjectDescriptor *descriptor) = 0;

	virtual WTexture2DRHIRef CreateTexture2D(RHITextureDescriptor* descriptor) = 0;

	virtual WTexture2DArrayRHIRef CreateTexture2DArray(RHITextureDescriptor* descriptor) = 0;

	virtual WTexture3DRHIRef CreateTexture3D(RHITextureDescriptor* descriptor) = 0;

	virtual RHISampler* CreateSampler(RHISamplerDescriptor * descriptor) = 0;

	virtual RHIRenderTarget* CreateRenderTarget(RHIRenderTargetDescriptor *descriptor) = 0;

	virtual WVertexBufferRHIRef CreateVertexBuffer(RHIBufferDescriptor* descriptor) = 0;

	virtual WDynamicVertexBufferRHIRef CreateDynamicVertexBuffer(RHIBufferDescriptor* descriptor) = 0;

	virtual WIndexBufferRHIRef CreateIndexBuffer(RHIBufferDescriptor* descriptor) = 0;

	virtual WUniformBufferRHIRef CreateUniformBuffer(RHIBufferDescriptor* descriptor) = 0;

	virtual WDynamicUniformBufferRHIRef CreateDynamicUniformBuffer(RHIBufferDescriptor* descriptor) = 0;

	virtual RHIGroup* CreateResourceGroup(RHIGroupDescriptor *descriptor) = 0;

	virtual RHIGroupLayout* CreateGroupLayout(RHIGroupLayoutDescriptor *descriptor) = 0;

	virtual RHIPipelineResourceLayout* CreatePipelineResourceLayout(RHIPipelineResourceLayoutDescriptor *descriptor) = 0;

	virtual RHIGroupPool* CreateGroupPool(RHIGroupPoolDescriptor *descriptor) = 0;

	virtual RHIScissor* CreateScissor(RHIScissorDescriptor *descriptor) = 0;

	virtual RHIViewport* CreateViewport(RHIViewportDescriptor *descriptor) = 0;

	virtual void UpdateUniformResourceToGroup(RHIUpdateResourceDescriptor *descriptor) = 0;

	virtual void UpdateTextureResourceToGroup(RHIUpdateResourceDescriptor *descriptor) = 0;

	virtual WEngine::WArray<RHISemaphore*> GetSemaphore(unsigned int count) = 0;

	virtual void WaitForFences(RHIFence *pFences, unsigned int count, bool waitForAll = true) = 0;

	virtual void ResetFences(RHIFence *pFences, unsigned int count) = 0;

	virtual int GetNextImage(RHISwapchain *swapchain, RHISemaphore *semaphore) = 0;

	virtual void Wait() = 0;

};