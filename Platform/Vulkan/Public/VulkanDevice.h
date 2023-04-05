#pragma once
#include "RHI/Public/RHIDevice.h"

class RHIQueue;
class RHISwapchain;
class RHISwapchainDescriptor;
struct QueueStack;

namespace Vulkan
{

	class VulkanDevice : public RHIDevice
	{
	public:
	
		VulkanDevice(class VulkanGPU* pInGPU, VkDeviceCreateInfo *pInfo, WEngine::WArray<QueueStack>& InQueueStack);

		virtual ~VulkanDevice();

		virtual unsigned int GetQueueCount(RHIQueueType type) override;

		virtual RHIQueue* GetQueue(RHIQueueType type, unsigned int count) override;

		RHISwapchain* CreateSwapchain(RHISwapchainDescriptor* descriptor, WEngine::WArray<VkImage>& OutImages);

		virtual WEngine::WArray<RHIFence*> CreateFence(unsigned int count) override;

		virtual RHIEvent* GetEvent() override;

		virtual WVertexShaderRHIRef CreateVertexShader(RHIShaderDescriptor* descriptor) override;

		virtual WPixelShaderRHIRef CreatePixelShader(RHIShaderDescriptor* descriptor) override;

		virtual WGeometryShaderRHIRef CreateGeometryShader(RHIShaderDescriptor* descriptor) override;

		virtual WComputeShaderRHIRef CreateComputeShader(RHIShaderDescriptor* descriptor) override;

		virtual WRenderPassRHIRef GetOrCreateRenderPass(RHIRenderPassDescriptor *descriptor) override;

		virtual WFramebufferRHIRef GetOrCreateFramebuffer(RHIFramebufferDescriptor *descriptor, RHIRenderPass* RenderPass) override;

		virtual WPsoRHIRef GetOrCreateGraphicsPipelineState(RHIGraphicsPipelineStateDescriptor *descriptor) override;

		virtual WTexture2DRHIRef CreateTexture2D(RHITextureDescriptor* descriptor) override;

		virtual WTexture2DArrayRHIRef CreateTexture2DArray(RHITextureDescriptor* descriptor) override;

		virtual WTexture3DRHIRef CreateTexture3D(RHITextureDescriptor* descriptor) override;

		virtual WTextureViewRHIRef CreateTextureView(RHITextureViewDescriptor* descriptor, RHITexture* InTexture) override;

		WTextureViewRHIRef CreateTextureView(RHITextureViewDescriptor* descriptor, VkImage InImage);

		virtual RHISampler* CreateSampler(RHISamplerDescriptor * descriptor) override;

		virtual WVertexBufferRHIRef CreateVertexBuffer(RHIBufferDescriptor* descriptor) override;

		virtual WDynamicVertexBufferRHIRef CreateDynamicVertexBuffer(RHIBufferDescriptor* descriptor) override;

		virtual WIndexBufferRHIRef CreateIndexBuffer(RHIBufferDescriptor* descriptor) override;

		virtual WUniformBufferRHIRef CreateUniformBuffer(RHIBufferDescriptor* descriptor) override;

		virtual WDynamicUniformBufferRHIRef CreateDynamicUniformBuffer(RHIBufferDescriptor *descriptor) override;

		virtual RHIScissor* CreateScissor(RHIScissorDescriptor *descriptor) override;

		virtual WViewportRHIRef CreateViewport(RHIViewportDescriptor *descriptor) override;

		virtual void UpdateUniformResourceToGroup(RHIUpdateResourceDescriptor *descriptor) override;

		virtual void UpdateTextureResourceToGroup(RHIUpdateResourceDescriptor *descriptor) override;

		virtual RHISemaphore* GetSemaphore() override;

		virtual void WaitForFences(RHIFence* pFences, unsigned int count, bool waitForAll = true) override;

		virtual void ResetFences(RHIFence* pFences, unsigned int count) override;

		virtual void SubmitCommandsAndFlushGPU() override;

		virtual void PrepareForCPURead() override;

		virtual void Wait() override;

		VulkanGPU* GetGPU() const { return pGPU; }

		VkDevice GetHandle() const { return Device; }

		class VulkanMemoryManager* GetMemoryManager() const { return pMemoryManager; }

		class VulkanStagingBufferManager* GetStagingBufferManager() const { return pStagingBufferManager; }

	private:

		VkImageUsageFlags GetImageUsage(ETextureCreateFlags Flag);

	private:

		VkDevice Device;

		VulkanMemoryManager *pMemoryManager;

		VulkanStagingBufferManager *pStagingBufferManager;

		WEngine::WArray<QueueStack> m_queues;

		VulkanGPU *pGPU;

	};

}