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

		void AddWaitingSemaphore(uint32 WaitingStageMask, class VulkanSemaphore* pSemaphore);

		const WEngine::WArray<class VulkanSemaphore*>& GetWaitingSemaphores() { return WaitingSemaphores; }

		const WEngine::WArray<uint32>& GetWaitingStageMasks() { return WaitingStageMasks; }

		class VulkanFence* GetFence() const { return pFence; }

		virtual void BeginPass(RHIRenderPassBeginDescriptor* descriptor) override;

		virtual void SetViewport(const Viewport& viewports) override;

		virtual void SetScissor(const Scissor& scissor) override;

		virtual void SetLineWidth(float width) override;

		virtual void SetDepthTestEnable(bool enable) override;

		virtual void SetPolygonTopology(EPrimitiveTopology primitive) override;

		virtual void ClearRenderTarget(bool isClearColor, bool isClearDepth, glm::vec4 clearColor, float clearDepth /* = 0.0f */, unsigned int clearStencil /* = 1 */) override;

		virtual void BindVertexBuffer(RHIBuffer* pBuffer) override;

		virtual void BindIndexBuffer(RHIBuffer* pBuffer) override;

		virtual void DrawVertexArray() override;

		virtual void DrawIndexed(unsigned int indexeCount, unsigned int firstIndex, unsigned int instanceCount = 1, unsigned int firstInstance = 0) override;

		virtual void ResourceBarrier(unsigned int srcStage, unsigned int dstStage) override;

		virtual void ResourceBarrier(RHIBarrierDescriptor* pDescriptor) override;

		virtual void CopyBufferToImage(RHITexture* pTexture, RHIBuffer* pBuffer, unsigned int width, unsigned int height) override;

		virtual void SetEvent(RHIEvent* pEvent) override;

		virtual void NextSubpass() override;

		virtual void EndPass() override;

		virtual void Clear() override;

		VulkanCommandPool* GetOwner() const { return pCommandPool; }

		VkCommandBuffer GetHandle() const { return CommandBuffer; }

		void RefreshFenceState();

		bool IsSubmitted() const { return State == ECmdState::Submitted; }

		bool HasBegun() const { return State == ECmdState::IsInsideBegin; }

		bool IsOutsideRenderPass() const { return State != ECmdState::IsInsideRenderPass; }

	private:

		enum ECmdState : uint8
		{
			ReadyForBegin,
			IsInsideBegin,
			IsInsideRenderPass,
			HasEnded,
			Submitted,
			NeedRest,
		};

		VkCommandBuffer CommandBuffer;

		VulkanCommandPool *pCommandPool;

		VulkanDevice *pDevice;

		uint8 bHasPipeline : 1;
		uint8 bHasViewport : 1;
		uint8 bHasScissor : 1;

		WEngine::WArray<VkViewport> CurrentViewports;
		WEngine::WArray<VkRect2D> CurrentScissors;
		uint32 CurrentStencilReference;

		WEngine::WArray<class VulkanSemaphore*> WaitingSemaphores;

		WEngine::WArray<uint32> WaitingStageMasks;

		WEngine::WArray<class VulkanSemaphore*> SubmittedWaitingSemaphores;

		VulkanFence *pFence;

		ECmdState State;

		friend class VulkanCommandBufferManager;
		friend class VulkanCommandPool;
		friend class VulkanQueue;
		friend class VulkanDynamicContext;
		friend class VulkanPendingGfxState;
	};

	class VulkanCommandBufferManager : public RHIResource
	{
	public:

		VulkanCommandBufferManager(class VulkanDevice *pInDevice, class VulkanQueue *pInQueue);

		~VulkanCommandBufferManager();

		VulkanCommandBuffer* GetActiveCommandBuffer();

		VulkanCommandBuffer* GetImmediateCommandBuffer();

		void PrepareForNewActiveCmdBuffer();

		void SubmitActiveCommandBuffer(uint32 NumSignalSemaphore = 0u, VulkanSemaphore* pSemaphores = nullptr);

		void SubmitImmediateCommandBuffer(uint32 NumSignalSemaphore = 0u, VulkanSemaphore* pSemaphores = nullptr);

		void SubmitActiveCommandBufferFromPresent(VulkanSemaphore* SignaleSemaphore);

		void WaitForCommandBuffer(VulkanCommandBuffer *CmdBuffer, double Time = 10.0);

		bool HasPendingActiveCmdBuffer() const { return ActiveCmdBuffer != nullptr; }

		bool HasPendingImmediateCmdBuffer() const { return ImmediateCmdBuffer != nullptr; }

		void RefreshFenceState();

	private:

		VulkanDevice *pDevice;

		class VulkanQueue *pQueue;

		VulkanCommandPool *pCommandPool;

		VulkanCommandBuffer *ActiveCmdBuffer;

		VulkanCommandBuffer *ImmediateCmdBuffer;

		WEngine::WArray<class VulkanSemaphore*> RenderingDoneSemaphores;

		class VulkanSemaphore* pActiveSemaphore;

		WEngine::WArray<class VulkanSemaphore*> ImmediateDoneSemaphores;

		class VulkanSemaphore* pImmediateSemaphore;

	};

}