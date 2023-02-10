#pragma once
#include "RHI/Public/RHICommandBuffer.h"

namespace Vulkan
{

	class VulkanCommandBuffer : public RHICommandBuffer
	{
	public:

		VulkanCommandBuffer(class VulkanDevice *pInDevice, class VulkanCommandPool* pInCommandPool, VkCommandBufferAllocateInfo *pInfo);

		virtual ~VulkanCommandBuffer();

		virtual void BeginScopePass(const WEngine::WString& passName) override;

		virtual void EndScopePass() override;

		virtual void ExecuteCommandBuffer(RHICommandBuffer* pCommandBuffer) override;

		void AddWaitingSemaphore(uint32 WaitingStageMask, class VulkanSemaphore *pSemaphore);

		const WEngine::WArray<VkSemaphore>& GetWaitingSemaphores() { return WaitingSemaphores; }

		const WEngine::WArray<uint32> GetWaitingStageMasks() { return WaitingStageMasks; }

		const VulkanFence* GetFence() { return pFence; }

		virtual RHIGraphicsEncoder* GetGraphicsEncoder() override;

		virtual RHIComputeEncoder* GetComputeEncoder() override;

		virtual void Clear() override;

		VkCommandPool* GetOwner() const { return pCommandPool; }

		VkCommandBuffer GetHandle() const { return CommandBuffer; }

	private:

		VkCommandBuffer CommandBuffer;

		VkCommandPool *pCommandPool;

		VulkanDevice *pDevice;

		uint8 m_isSecondary : 1;

		WEngine::WArray<VkSemaphore> WaitingSemaphores;

		WEngine::WArray<uint32> WaitingStageMasks;

		class VulkanFence *pFence;

	};

	class VulkanCommandBufferManager : RHIResource
	{
	public:

		VulkanCommandBufferManager(class VulkanDevice *pInDevice);

		~VulkanCommandBufferManager();

		void SubmitActiveCommandBuffer(WEngine::WArray<class VulkanSemaphore*>& SignalSemaphores);

		void SubmitImmediateCommandBuffer(WEngine::WArray<VulkanSemaphore*>& SignalSemaphores);

		void WaitForCommandBuffer(VulkanCommandBuffer *CmdBuffer, double Time);

	private:

		VulkanDevice *pDevice;

		class VulkanQueue *pQueue;

		VulkanCommandPool *pCommandPool;

		VulkanCommandBuffer *ActiveCmdBuffer;

		VulkanCommandBuffer *ImmediateCmdBuffer;

		WEngine::WArray<class VulkanSemaphore*> RenderingDoneSemaphores;

		VulkanSemaphore *pActiveSemaphore;

		WEngine::WArray<class VulkanSemaphore*> ImmediateDoneSemaphores;

		VulkanSemaphore *pImmediateSemaphore;

	};

}