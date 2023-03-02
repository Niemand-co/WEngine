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

		void AddWaitingSemaphore(uint32 WaitingStageMask, WEngine::WSharedPtr<class VulkanSemaphore>& pSemaphore);

		const WEngine::WArray<WEngine::WSharedPtr<VulkanSemaphore>>& GetWaitingSemaphores() { return WaitingSemaphores; }

		const WEngine::WArray<uint32> GetWaitingStageMasks() { return WaitingStageMasks; }

		class VulkanFence* GetFence() const { return pFence; }

		virtual void BeginPass(RHIRenderPassBeginDescriptor* descriptor) override;

		virtual void SetPipeline(RHIPipelineStateObject* pso) override;

		virtual void SetViewport(const Viewport& viewports) override;

		virtual void SetScissor(const Scissor& scissor) override;

		virtual void SetLineWidth(float width) override;

		virtual void SetDepthTestEnable(bool enable) override;

		virtual void SetPolygonTopology(PrimitiveTopology primitive) override;

		virtual void ClearRenderTarget(bool isClearColor, bool isClearDepth, glm::vec4 clearColor, float clearDepth /* = 0.0f */, unsigned int clearStencil /* = 1 */) override;

		virtual void BindVertexBuffer(RHIBuffer* pBuffer) override;

		virtual void BindIndexBuffer(RHIBuffer* pBuffer) override;

		virtual void BindGroups(unsigned int groupCount, RHIGroup* pGroup, RHIPipelineResourceLayout* pPipelineResourceLayout, unsigned int offsetCount, unsigned int* offsets) override;

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

		uint8 m_isSecondary : 1;

		WEngine::WArray<WEngine::WSharedPtr<VulkanSemaphore>> WaitingSemaphores;

		WEngine::WArray<uint32> WaitingStageMasks;

		WEngine::WArray<WEngine::WSharedPtr<VulkanSemaphore>> SubmittedWaitingSemaphores;

		VulkanFence *pFence;

		ECmdState State;

		friend class VulkanCommandBufferManager;
		friend class VulkanCommandPool;
		friend class VulkanQueue;
		friend class VulkanContext;

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

		void SubmitActiveCommandBufferFromPresent(WEngine::WSharedPtr<VulkanSemaphore>& SignaleSemaphore);

		void WaitForCommandBuffer(VulkanCommandBuffer *CmdBuffer, double Time = 10.0);

		bool HasPendingActiveCmdBuffer() const { return ActiveCmdBuffer != nullptr; }

		bool HasPendingImmediateCmdBuffer() const { return ImmediateCmdBuffer != nullptr; }

	private:

		VulkanDevice *pDevice;

		class VulkanQueue *pQueue;

		VulkanCommandPool *pCommandPool;

		VulkanCommandBuffer *ActiveCmdBuffer;

		VulkanCommandBuffer *ImmediateCmdBuffer;

		WEngine::WArray<WEngine::WSharedPtr<class VulkanSemaphore>> RenderingDoneSemaphores;

		WEngine::WSharedPtr<class VulkanSemaphore> pActiveSemaphore;

		WEngine::WArray<WEngine::WSharedPtr<class VulkanSemaphore>> ImmediateDoneSemaphores;

		WEngine::WSharedPtr<class VulkanSemaphore> pImmediateSemaphore;

	};

}