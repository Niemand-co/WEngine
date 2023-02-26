#pragma once

class RHIDevice : public RHIResource
{
public:

	virtual ~RHIDevice() = default;

	virtual unsigned int GetQueueCount(RHIQueueType) = 0;

	virtual class RHIQueue* GetQueue(RHIQueueType, unsigned int index) = 0;

	virtual WEngine::WArray<class RHIFence*> CreateFence(unsigned int count) = 0;

	virtual class RHIEvent* GetEvent() = 0;

	virtual WVertexShaderRHIRef CreateVertexShader(class RHIShaderDescriptor* descriptor) = 0;

	virtual WPixelShaderRHIRef CreatePixelShader(class RHIShaderDescriptor* descriptor) = 0;

	virtual WGeometryShaderRHIRef CreateGeometryShader(class RHIShaderDescriptor* descriptor) = 0;

	virtual WComputeShaderRHIRef CreateComputeShader(class RHIShaderDescriptor* descriptor) = 0;

	virtual class RHIRenderPass* CreateRenderPass(class RHIRenderPassDescriptor *descriptor) = 0;

	virtual class RHIPipelineStateObject* CreatePipelineStateObject(class RHIPipelineStateObjectDescriptor *descriptor) = 0;

	virtual WTexture2DRHIRef CreateTexture2D(class RHITextureDescriptor* descriptor) = 0;

	virtual WTexture2DArrayRHIRef CreateTexture2DArray(class RHITextureDescriptor* descriptor) = 0;

	virtual WTexture3DRHIRef CreateTexture3D(class RHITextureDescriptor* descriptor) = 0;

	virtual WTextureViewRHIRef CreateTextureView(class RHITextureViewDescriptor* descriptor, RHITexture* InTexture) = 0;;

	virtual class RHISampler* CreateSampler(class RHISamplerDescriptor * descriptor) = 0;

	virtual WVertexBufferRHIRef CreateVertexBuffer(class RHIBufferDescriptor* descriptor) = 0;

	virtual WDynamicVertexBufferRHIRef CreateDynamicVertexBuffer(class RHIBufferDescriptor* descriptor) = 0;

	virtual WIndexBufferRHIRef CreateIndexBuffer(class RHIBufferDescriptor* descriptor) = 0;

	virtual WUniformBufferRHIRef CreateUniformBuffer(class RHIBufferDescriptor* descriptor) = 0;

	virtual WDynamicUniformBufferRHIRef CreateDynamicUniformBuffer(class RHIBufferDescriptor* descriptor) = 0;

	virtual class RHIGroup* CreateResourceGroup(class RHIGroupDescriptor *descriptor) = 0;

	virtual class RHIGroupLayout* CreateGroupLayout(class RHIGroupLayoutDescriptor *descriptor) = 0;

	virtual class RHIPipelineResourceLayout* CreatePipelineResourceLayout(class RHIPipelineResourceLayoutDescriptor *descriptor) = 0;

	virtual class RHIGroupPool* CreateGroupPool(class RHIGroupPoolDescriptor *descriptor) = 0;

	virtual class RHIScissor* CreateScissor(class RHIScissorDescriptor *descriptor) = 0;

	virtual WViewportRHIRef CreateViewport(class RHIViewportDescriptor* descriptor) = 0;

	virtual void UpdateUniformResourceToGroup(class RHIUpdateResourceDescriptor *descriptor) = 0;

	virtual void UpdateTextureResourceToGroup(class RHIUpdateResourceDescriptor *descriptor) = 0;

	virtual class RHISemaphore* GetSemaphore() = 0;

	virtual void WaitForFences(class RHIFence *pFences, unsigned int count, bool waitForAll = true) = 0;

	virtual void ResetFences(class RHIFence *pFences, unsigned int count) = 0;

	virtual void SubmitCommandsAndFlushGPU() = 0;

	virtual void Wait() = 0;

};