#include "pch.h"
#include "Platform/Vulkan/Public/VulkanHeads.h"
#include "RHI/Public/RHIHeads.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"

namespace Vulkan
{

	VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice* pInDevice, VulkanCommandPool *pInCommandPool, VkCommandBufferAllocateInfo* pInfo)
		: pDevice(pInDevice),
		  pCommandPool(pInCommandPool)
	{
		vkAllocateCommandBuffers(pDevice->GetHandle(), pInfo, &CommandBuffer);
		pFence = new VulkanFence(pDevice);
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		vkFreeCommandBuffers(pDevice->GetHandle(), pCommandPool->GetHandle(), 1, &CommandBuffer);
		delete pFence;
	}

	void VulkanCommandBuffer::BeginScopePass(const WEngine::WString& passName)
	{
		VkCommandBufferBeginInfo CommandbufferBeginInfo = {};
		CommandbufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		CommandbufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		VkCommandBufferInheritanceInfo inheritanceInfo = {};

		RE_ASSERT(vkBeginCommandBuffer(CommandBuffer, &CommandbufferBeginInfo) == VK_SUCCESS, "Failed to Begin Command Buffer.");

		State = ECmdState::IsInsideBegin;
	}

	void VulkanCommandBuffer::EndScopePass()
	{
		vkEndCommandBuffer(CommandBuffer);
		State = ECmdState::HasEnded;
	}

	void VulkanCommandBuffer::ExecuteCommandBuffer(RHICommandBuffer* pCommandBuffer)
	{
		VkCommandBuffer CmdBuffer = static_cast<VulkanCommandBuffer*>(pCommandBuffer)->GetHandle();
		vkCmdExecuteCommands(CommandBuffer, 1, &CmdBuffer);
	}

	void VulkanCommandBuffer::AddWaitingSemaphore(uint32 WaitingStageMask, VulkanSemaphore* pSemaphore)
	{
		WaitingStageMasks.Push(WaitingStageMask);
		WaitingSemaphores.Push(pSemaphore);
	}

	void VulkanCommandBuffer::BeginPass(RHIRenderPassBeginDescriptor* descriptor)
	{
		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.framebuffer = *static_cast<VulkanRenderTarget*>(descriptor->renderTarget)->GetHandle();
		renderPassBeginInfo.renderPass = static_cast<VulkanRenderPass*>(descriptor->renderPass)->GetHandle();
		renderPassBeginInfo.renderArea.offset = { 0, 0 };
		renderPassBeginInfo.renderArea.extent = { descriptor->renderTarget->GetWidth(), descriptor->renderTarget->GetHeight() };
		renderPassBeginInfo.clearValueCount = descriptor->clearCount;

		WEngine::WArray<VkClearValue> values(descriptor->clearCount);
		for (unsigned int i = 0; i < descriptor->clearCount; ++i)
		{
			values[i] = { descriptor->pClearValues[i].Color[0], descriptor->pClearValues[i].Color[1], descriptor->pClearValues[i].Color[2], descriptor->pClearValues[i].Color[3] };
		}
		renderPassBeginInfo.pClearValues = values.GetData();

		vkCmdBeginRenderPass(CommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		State = ECmdState::IsInsideRenderPass;
	}

	void VulkanCommandBuffer::SetViewport(const Viewport& pViewport)
	{
		vkCmdSetViewport(CommandBuffer, 0, 1, {});
	}

	void VulkanCommandBuffer::SetScissor(const Scissor& scissor)
	{
		vkCmdSetScissor(CommandBuffer, 0, 1, {});
	}

	void VulkanCommandBuffer::SetLineWidth(float width)
	{
		vkCmdSetLineWidth(CommandBuffer, width);
	}

	void VulkanCommandBuffer::SetDepthTestEnable(bool enable)
	{
		vkCmdSetDepthTestEnable(CommandBuffer, enable);
	}

	void VulkanCommandBuffer::SetPolygonTopology(EPrimitiveTopology primitive)
	{
		vkCmdSetPrimitiveTopology(CommandBuffer, WEngine::ToVulkan(primitive));
	}

	void VulkanCommandBuffer::ClearRenderTarget(bool isClearColor, bool isClearDepth, glm::vec4 clearColor, float clearDepth, unsigned int clearStencil)
	{

	}

	void VulkanCommandBuffer::BindVertexBuffer(RHIBuffer* pBuffer)
	{
		VkDeviceSize offets[] = { 0 };
		VkBuffer Buffer = static_cast<VulkanVertexBuffer*>(pBuffer)->GetHandle();
		vkCmdBindVertexBuffers(CommandBuffer, 0, 1, &Buffer, offets);
	}

	void VulkanCommandBuffer::BindIndexBuffer(RHIBuffer* pBuffer)
	{
		vkCmdBindIndexBuffer(CommandBuffer, static_cast<VulkanIndexBuffer*>(pBuffer)->GetHandle(), 0, VK_INDEX_TYPE_UINT32);
	}

	void VulkanCommandBuffer::DrawVertexArray()
	{
		vkCmdDraw(CommandBuffer, 3, 1, 0, 0);
	}

	void VulkanCommandBuffer::DrawIndexed(unsigned int indexCount, unsigned int firstIndex, unsigned int instanceCount, unsigned int firstInstance)
	{
		vkCmdDrawIndexed(CommandBuffer, indexCount, instanceCount, firstIndex, 0, firstInstance);
	}

	void VulkanCommandBuffer::ResourceBarrier(unsigned int srcStage, unsigned int dstStage)
	{
		vkCmdPipelineBarrier(CommandBuffer, srcStage, dstStage, VkDependencyFlagBits(), 0, nullptr, 0, nullptr, 0, nullptr);
	}

	void VulkanCommandBuffer::ResourceBarrier(RHIBarrierDescriptor* pDescriptor)
	{

	}

	void VulkanCommandBuffer::CopyBufferToImage(RHITexture* pTexture, RHIBuffer* pBuffer, unsigned int width, unsigned int height)
	{
		VkBufferImageCopy bufferImageCopy = {};
		{
			bufferImageCopy.bufferOffset = 0;
			bufferImageCopy.bufferImageHeight = 0;
			bufferImageCopy.bufferRowLength = 0;

			bufferImageCopy.imageExtent = { width, height, 1 };
			bufferImageCopy.imageOffset = { 0, 0, 0 };

			bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			bufferImageCopy.imageSubresource.baseArrayLayer = 0;
			bufferImageCopy.imageSubresource.mipLevel = 0;
			bufferImageCopy.imageSubresource.layerCount = 1;
		}
		vkCmdCopyBufferToImage(CommandBuffer, static_cast<VulkanTextureBuffer*>(pBuffer)->GetHandle(), static_cast<VulkanTexture2D*>(pTexture)->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);
	}

	void VulkanCommandBuffer::SetEvent(RHIEvent* pEvent)
	{
		vkCmdSetEvent(CommandBuffer, *static_cast<VulkanEvent*>(pEvent)->GetHandle(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
	}

	void VulkanCommandBuffer::NextSubpass()
	{
		vkCmdNextSubpass(CommandBuffer, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanCommandBuffer::EndPass()
	{
		vkCmdEndRenderPass(CommandBuffer);
		State = ECmdState::IsInsideBegin;
	}

	void VulkanCommandBuffer::Clear()
	{
		vkResetCommandBuffer(CommandBuffer, 0);
	}

	void VulkanCommandBuffer::RefreshFenceState()
	{
		if (State == ECmdState::Submitted)
		{
			if (pFence->IsFenceSignaled())
			{
				//for (VulkanSemaphore* Semaphore : SubmittedWaitingSemaphores)
				//{
				//	delete Semaphore;
				//}
				SubmittedWaitingSemaphores.Clear();

				pFence->Reset();

				State = ECmdState::NeedRest;
			}
		}
	}

	VulkanCommandBufferManager::VulkanCommandBufferManager(VulkanDevice* pInDevice, VulkanQueue *pInQueue)
		: pDevice(pInDevice),
		  pQueue(pInQueue),
		  ImmediateCmdBuffer(nullptr),
		  pImmediateSemaphore(nullptr)
	{
		pCommandPool = (VulkanCommandPool*)pQueue->GetCommandPool(this);
		ActiveCmdBuffer = (VulkanCommandBuffer*)pCommandPool->GetCommandBuffer();
		PrepareForNewActiveCmdBuffer();
		pActiveSemaphore = new VulkanSemaphore(pInDevice);
	}

	VulkanCommandBufferManager::~VulkanCommandBufferManager()
	{
		delete pCommandPool;
		delete pQueue;
	}

	VulkanCommandBuffer* VulkanCommandBufferManager::GetActiveCommandBuffer()
	{
		if (ImmediateCmdBuffer)
		{
			SubmitImmediateCommandBuffer();
		}
		return ActiveCmdBuffer;
	}

	VulkanCommandBuffer* VulkanCommandBufferManager::GetImmediateCommandBuffer()
	{	
		WEngine::WScopeLock(&pCommandPool->CS);
		if (!ImmediateCmdBuffer)
		{
			pImmediateSemaphore = new VulkanSemaphore(pDevice);

			for (uint32 BufferIndex = 0; BufferIndex < pCommandPool->CmdBuffers.Size(); ++BufferIndex)
			{
				pCommandPool->CmdBuffers[BufferIndex]->RefreshFenceState();

				if (pCommandPool->CmdBuffers[BufferIndex]->State == VulkanCommandBuffer::ECmdState::ReadyForBegin || pCommandPool->CmdBuffers[BufferIndex]->State == VulkanCommandBuffer::ECmdState::NeedRest)
				{
					ImmediateCmdBuffer = pCommandPool->CmdBuffers[BufferIndex];
					ImmediateCmdBuffer->BeginScopePass("None");
					return ImmediateCmdBuffer;
				}
			}

			ImmediateCmdBuffer = pCommandPool->AllocateCmdBuffer();
			ImmediateCmdBuffer->BeginScopePass("None");
		}

		return ImmediateCmdBuffer;
	}

	void VulkanCommandBufferManager::PrepareForNewActiveCmdBuffer()
	{
		WEngine::WScopeLock(&pCommandPool->CS);
		pActiveSemaphore = new VulkanSemaphore(pDevice);

		for (uint32 BufferIndex = 0; BufferIndex < pCommandPool->CmdBuffers.Size(); ++BufferIndex)
		{
			VulkanCommandBuffer *CmdBuffer = pCommandPool->CmdBuffers[BufferIndex];
			CmdBuffer->RefreshFenceState();

			if (CmdBuffer->State == VulkanCommandBuffer::ECmdState::ReadyForBegin || CmdBuffer->State == VulkanCommandBuffer::ECmdState::NeedRest)
			{
				ActiveCmdBuffer = CmdBuffer;
				ActiveCmdBuffer->BeginScopePass("None");
				return;
			}
		}

		ActiveCmdBuffer = pCommandPool->AllocateCmdBuffer();
		ActiveCmdBuffer->BeginScopePass("None");
		
	}

	void VulkanCommandBufferManager::SubmitActiveCommandBuffer(uint32 NumSignalSemaphore, VulkanSemaphore* pSemaphores)
	{
		WEngine::WScopeLock(&pCommandPool->CS);
		if(!ActiveCmdBuffer->IsSubmitted() && ActiveCmdBuffer->HasBegun())
		{
			if (!ActiveCmdBuffer->IsOutsideRenderPass())
			{
				ActiveCmdBuffer->EndPass();
			}

			ActiveCmdBuffer->EndScopePass();

			for (VulkanSemaphore* IMSemaphore : ImmediateDoneSemaphores)
			{
				ActiveCmdBuffer->AddWaitingSemaphore(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, IMSemaphore);
			}
			ImmediateDoneSemaphores.Clear();

			WEngine::WArray<VkSemaphore> Semaphores;
			Semaphores.Reserve(NumSignalSemaphore + 1);
			for (uint32 SemaphoreIndex = 0; SemaphoreIndex < NumSignalSemaphore; ++SemaphoreIndex)
			{
				Semaphores.Push(pSemaphores[SemaphoreIndex].GetHandle());
			}
			Semaphores.Push(pActiveSemaphore->GetHandle());
			pActiveSemaphore = nullptr;

			pQueue->Submit(ActiveCmdBuffer, Semaphores.Size(), Semaphores.GetData());
		}

		ActiveCmdBuffer = nullptr;
	}

	void VulkanCommandBufferManager::SubmitImmediateCommandBuffer(uint32 NumSignalSemaphore, VulkanSemaphore *pSemaphores)
	{
		WEngine::WScopeLock(&pCommandPool->CS);
		if (!ImmediateCmdBuffer->IsSubmitted() && ImmediateCmdBuffer->HasBegun())
		{
			ImmediateCmdBuffer->EndScopePass();

			for (VulkanSemaphore* pSemaphore : RenderingDoneSemaphores)
			{
				ImmediateCmdBuffer->AddWaitingSemaphore(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, pSemaphore);
			}
			RenderingDoneSemaphores.Clear();

			WEngine::WArray<VkSemaphore> Semaphores(NumSignalSemaphore);
			for (uint32 SemaphoreIndex = 0; SemaphoreIndex < NumSignalSemaphore; ++SemaphoreIndex)
			{
				Semaphores[SemaphoreIndex] = pSemaphores[SemaphoreIndex].GetHandle();
			}
			Semaphores.Push(pImmediateSemaphore->GetHandle());
			ImmediateDoneSemaphores.Push(pImmediateSemaphore);
			pImmediateSemaphore = nullptr;

			pQueue->Submit(ImmediateCmdBuffer, Semaphores.Size(), Semaphores.GetData());
		}

		ImmediateCmdBuffer = nullptr;
	}

	void VulkanCommandBufferManager::SubmitActiveCommandBufferFromPresent(VulkanSemaphore* SignalSemaphore)
	{
		WEngine::WScopeLock(&pCommandPool->CS);
		if (SignalSemaphore)
		{
			VkSemaphore Semaphores[2] = { pActiveSemaphore->GetHandle(), SignalSemaphore->GetHandle() };
			pQueue->Submit(ActiveCmdBuffer, 2, Semaphores);
		}
		else
		{
			VkSemaphore Semaphore = pActiveSemaphore->GetHandle();
			pQueue->Submit(ActiveCmdBuffer, 1, &Semaphore);
		}
		RenderingDoneSemaphores.Push(pActiveSemaphore);
		pActiveSemaphore = nullptr;
	}

	void VulkanCommandBufferManager::WaitForCommandBuffer(VulkanCommandBuffer* CmdBuffer, double Time)
	{
		CmdBuffer->GetFence()->Wait(Time);
		CmdBuffer->RefreshFenceState();
	}

	void VulkanCommandBufferManager::RefreshFenceState()
	{
		for (uint32 Index = 0; Index < pCommandPool->CmdBuffers.Size(); ++Index)
		{
			pCommandPool->CmdBuffers[Index]->RefreshFenceState();
		}
	}

}