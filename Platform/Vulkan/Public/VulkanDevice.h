#pragma once
#include "RHI/Public/RHIDevice.h"

class RHIQueue;
class VulkanTextureBase;
struct QueueStack;

namespace Vulkan
{

	class VulkanGPU;

	class VulkanDevice : public RHIDevice
	{
	public:
	
		VulkanDevice(VkDevice *device, VulkanGPU *pGPU, WEngine::WArray<QueueStack> stacks);

		virtual ~VulkanDevice();

		virtual unsigned int GetQueueCount(RHIQueueType type) override;

		virtual RHIQueue* GetQueue(RHIQueueType type, unsigned int count) override;

		virtual RHISwapchain* CreateSwapchain(RHISwapchainDescriptor* descriptor) override;

		virtual WEngine::WArray<RHIFence*> CreateFence(unsigned int count) override;

		virtual RHIEvent* GetEvent() override;

		virtual RHIShader* CreateVertexShader(RHIShaderDescriptor* descriptor) override;

		virtual RHIShader* CreateGeometryShader(RHIShaderDescriptor* descriptor) override;

		virtual RHIShader* CreatePixelShader(RHIShaderDescriptor* descriptor) override;

		virtual RHIRenderPass* CreateRenderPass(RHIRenderPassDescriptor *descriptor) override;

		virtual RHIPipelineStateObject* CreatePipelineStateObject(RHIPipelineStateObjectDescriptor *descriptor) override;

		virtual RHITexture* CreateTexture(RHITextureDescriptor *descriptor) override;

		virtual RHISampler* CreateSampler(RHISamplerDescriptor * descriptor) override;

		virtual RHIRenderTarget* CreateRenderTarget(RHIRenderTargetDescriptor *descriptor) override;

		virtual RHIBuffer* CreateVertexBuffer(RHIBufferDescriptor* descriptor) override;

		virtual RHIBuffer* CreateDynamicVertexBuffer(RHIBufferDescriptor* descriptor) override;

		virtual RHIBuffer* CreateIndexBuffer(RHIBufferDescriptor* descriptor) override;

		virtual RHIBuffer* CreateUniformBuffer(RHIBufferDescriptor* descriptor) override;

		virtual RHIBuffer* CreateDynamicUniformBuffer(RHIBufferDescriptor *descriptor) override;

		virtual RHIGroup* CreateResourceGroup(RHIGroupDescriptor *descriptor) override;

		virtual RHIGroupLayout* CreateGroupLayout(RHIGroupLayoutDescriptor *descriptor) override;

		virtual RHIPipelineResourceLayout* CreatePipelineResourceLayout(RHIPipelineResourceLayoutDescriptor *descriptor) override;

		virtual RHIGroupPool* CreateGroupPool(RHIGroupPoolDescriptor *descriptor) override;

		virtual RHIScissor* CreateScissor(RHIScissorDescriptor *descriptor) override;

		virtual RHIViewport* CreateViewport(RHIViewportDescriptor *descriptor) override;

		virtual void UpdateUniformResourceToGroup(RHIUpdateResourceDescriptor *descriptor) override;

		virtual void UpdateTextureResourceToGroup(RHIUpdateResourceDescriptor *descriptor) override;

		virtual WEngine::WArray<RHISemaphore*> GetSemaphore(unsigned int count) override;

		virtual void WaitForFences(RHIFence* pFences, unsigned int count, bool waitForAll = true) override;

		virtual void ResetFences(RHIFence* pFences, unsigned int count) override;

		virtual int GetNextImage(RHISwapchain *swapchain, RHISemaphore *semaphore) override;

		virtual void Wait() override;

		VulkanGPU* GetGPU() const { return m_pGPU; }

		VkDevice* GetHandle() const { return m_pDevice; }

	private:

		VkDevice *m_pDevice;

		WEngine::WArray<QueueStack> m_queues;

		VulkanGPU *m_pGPU;

	};

}