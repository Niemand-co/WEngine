#pragma once

class RHIQueue;
class RHISwapchain;
class RHIFence;
class RHIShader;
class RHIRenderPass;
class RHIPipelineStateObject;
class RHITexture;
class RHIRenderTarget;
class RHISemaphore;
class RHIBuffer;

class RHISwapchainDescriptor;
class RHIShaderDescriptor;
class RHIRenderPassDescriptor;
class RHIPipelineStateObjectDescriptor;
class RHITextureDescriptor;
class RHIRenderTargetDescriptor;
class RHIBufferDescriptor;

class RHIDevice
{
public:

	virtual ~RHIDevice() = default;

	virtual unsigned int GetQueueCount(RHIQueueType) = 0;

	virtual RHIQueue* GetQueue(RHIQueueType, unsigned int index) = 0;

	virtual RHISwapchain* CreateSwapchain(RHISwapchainDescriptor *descriptor) = 0;

	virtual RHIFence* CreateFence() = 0;

	virtual RHIShader* CreateShader(RHIShaderDescriptor *descriptor) = 0;

	virtual RHIRenderPass* CreateRenderPass(RHIRenderPassDescriptor *descriptor) = 0;

	virtual RHIPipelineStateObject* CreatePipelineStateObject(RHIPipelineStateObjectDescriptor *descriptor) = 0;

	virtual RHITexture* CreateTexture(RHITextureDescriptor *descriptor) = 0;

	virtual RHIRenderTarget* CreateRenderTarget(RHIRenderTargetDescriptor *descriptor) = 0;

	virtual RHIBuffer* CreateBuffer(RHIBufferDescriptor *descriptor) = 0;

	virtual std::vector<RHISemaphore*> GetSemaphore(unsigned int count) = 0;

	virtual int GetNextImage(RHISwapchain *swapchain, RHISemaphore *semaphore) = 0;

};