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

		virtual WShaderRHIRef CreateShader(RHIShaderDescriptor* descriptor) override;

		virtual WTextureRHIRef CreateTexture(const RHITextureDesc& InDesc) override;

		virtual WVertexBufferRHIRef CreateVertexBuffer(RHIBufferDescriptor* descriptor) override;

		virtual WIndexBufferRHIRef CreateIndexBuffer(RHIBufferDescriptor* descriptor) override;

		virtual WUniformBufferRHIRef CreateUniformBuffer(uint8* InContents, const ShaderParametersLayout* InLayout, EUniformBufferUsage InUsage) override;

		virtual RHIScissor* CreateScissor(RHIScissorDescriptor *descriptor) override;

		virtual WViewportRHIRef CreateViewport(RHIViewportDescriptor *descriptor) override;

		virtual RHISemaphore* GetSemaphore() override;

		virtual void WaitForFences(RHIFence* pFences, unsigned int count, bool waitForAll = true) override;

		virtual void ResetFences(RHIFence* pFences, unsigned int count) override;

		virtual void SubmitCommandsAndFlushGPU() override;

		virtual void PrepareForCPURead() override;

		virtual void Wait() override;

		VulkanGPU* GetGPU() const { return pGPU; }

		VkPhysicalDeviceLimits GetLimits() const { return GPUProps.limits; }

		VkDevice GetHandle() const { return Device; }

		class VulkanMemoryManager* GetMemoryManager() const { return pMemoryManager; }

		class VulkanStagingBufferManager* GetStagingBufferManager() const { return pStagingBufferManager; }

		class VulkanPipelineStateManager* GetPipelineStateManager() const { return pPipelineStateManager; }

		class VulkanSamplerStateManager* GetSamplerStateManager() const { return pSamplerStateManager; }

		class VulkanLayoutManager* GetLayoutManager() const { return pLayoutManager; }

	private:

		VkImageUsageFlags GetImageUsage(ETextureCreateFlags Flag);

	private:

		VkDevice Device;

		VulkanMemoryManager *pMemoryManager;

		VulkanStagingBufferManager *pStagingBufferManager;

		VulkanPipelineStateManager *pPipelineStateManager;

		VulkanSamplerStateManager *pSamplerStateManager;

		VulkanLayoutManager *pLayoutManager;

		WEngine::WArray<QueueStack> m_queues;

		VkPhysicalDeviceProperties GPUProps;

		VulkanGPU *pGPU;

	};

}