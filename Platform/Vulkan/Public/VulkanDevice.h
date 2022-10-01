#pragma once
#include "RHI/Public/RHIDevice.h"

class RHIQueue;
class VulkanTexture;
struct QueueStack;

namespace Vulkan
{

	class VulkanDevice : public RHIDevice
	{
	public:
	
		VulkanDevice(VkDevice *device, std::vector<QueueStack> stacks);

		virtual ~VulkanDevice();

		virtual unsigned int GetQueueCount(RHIQueueType type) override;

		virtual RHIQueue* GetQueue(RHIQueueType type, unsigned int count) override;

		virtual RHISwapchain* CreateSwapchain(RHISwapchainDescriptor* descriptor) override;

		virtual RHISwapchain* RecreateSwapchain(RHISwapchain *&swapchain, RHISwapchainDescriptor *descriptor) override;

		virtual std::vector<RHIFence*> CreateFence(unsigned int count) override;

		virtual RHIShader* CreateShader(RHIShaderDescriptor *descriptor) override;

		virtual RHIRenderPass* CreateRenderPass(RHIRenderPassDescriptor *descriptor) override;

		virtual RHIPipelineStateObject* CreatePipelineStateObject(RHIPipelineStateObjectDescriptor *descriptor) override;

		virtual RHITexture* CreateTexture(RHITextureDescriptor *descriptor) override;

		virtual RHIRenderTarget* CreateRenderTarget(RHIRenderTargetDescriptor *descriptor) override;

		virtual RHIBuffer* CreateBuffer(RHIBufferDescriptor *descriptor) override;

		virtual std::vector<RHISemaphore*> GetSemaphore(unsigned int count) override;

		virtual void WaitForFences(RHIFence* pFences, unsigned int count, bool waitForAll = true) override;

		virtual void ResetFences(RHIFence* pFences, unsigned int count) override;

		virtual int GetNextImage(RHISwapchain *swapchain, RHISemaphore *semaphore) override;

		VkDevice* GetHandle();

	private:

		VkDevice *m_device;

		std::vector<QueueStack> m_queues;

	};

}