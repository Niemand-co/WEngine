#include "pch.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanQueue.h"
#include "Platform/Vulkan/Public/VulkanCommandPool.h"
#include "Platform/Vulkan/Public/VulkanSemaphore.h"
#include "Platform/Vulkan/Public/VulkanFence.h"
#include "Platform/Vulkan/Encoder/Public/VulkanGraphicsEncoder.h"
#include "Platform/Vulkan/Encoder/Public/VulkanComputeEncoder.h"

namespace Vulkan
{

	VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice* pInDevice, VulkanCommandPool *pInCommandPool, VkCommandBufferAllocateInfo* pInfo)
		: pDevice(pInDevice)
	{
		vkAllocateCommandBuffers(pDevice->GetHandle(), pInfo, &CommandBuffer);
		pFence = new VulkanFence(pDevice);
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		vkFreeCommandBuffers(pDevice->GetHandle(), *pCommandPool, 1, &CommandBuffer);
	}

	void VulkanCommandBuffer::BeginScopePass(const WEngine::WString& passName)
	{
		VkCommandBufferBeginInfo CommandbufferBeginInfo = {};
		CommandbufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		CommandbufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		VkCommandBufferInheritanceInfo inheritanceInfo = {};

		RE_ASSERT(vkBeginCommandBuffer(CommandBuffer, &CommandbufferBeginInfo) == VK_SUCCESS, "Failed to Begin Command Buffer.");
	}

	void VulkanCommandBuffer::EndScopePass()
	{
		vkEndCommandBuffer(CommandBuffer);
	}

	void VulkanCommandBuffer::ExecuteCommandBuffer(RHICommandBuffer* pCommandBuffer)
	{
		VkCommandBuffer CmdBuffer = static_cast<VulkanCommandBuffer*>(pCommandBuffer)->GetHandle();
		vkCmdExecuteCommands(CommandBuffer, 1, &CmdBuffer);
	}

	void VulkanCommandBuffer::AddWaitingSemaphore(uint32 WaitingStageMask, VulkanSemaphore* pSemaphore)
	{
		WaitingStageMasks.Push(WaitingStageMask);
		WaitingSemaphores.Push(pSemaphore->GetHandle());
	}

	RHIGraphicsEncoder* VulkanCommandBuffer::GetGraphicsEncoder()
	{
		return new VulkanGraphicsEncoder(&CommandBuffer);
	}

	RHIComputeEncoder* VulkanCommandBuffer::GetComputeEncoder()
	{
		return nullptr;
	}

	void VulkanCommandBuffer::Clear()
	{
		vkResetCommandBuffer(CommandBuffer, 0);
	}

	VulkanCommandBufferManager::VulkanCommandBufferManager(VulkanDevice* pInDevice)
		: pDevice(pInDevice)
	{
		pQueue = (VulkanQueue*)pInDevice->GetQueue(RHIQueueType((uint8)RHIQueueType::Graphics | (uint8)RHIQueueType::Present), 1);
		pCommandPool = (VulkanCommandPool*)pQueue->GetCommandPool();
		ActiveCmdBuffer = (VulkanCommandBuffer*)pCommandPool->GetCommandBuffer(true);
		ImmediateCmdBuffer = (VulkanCommandBuffer*)pCommandPool->GetCommandBuffer(true);
	}

	VulkanCommandBufferManager::~VulkanCommandBufferManager()
	{
		delete pCommandPool;
		delete pQueue;
	}

	VulkanCommandBuffer* VulkanCommandBufferManager::GetActiveCommandBuffer()
	{
		if (!ActiveCmdBuffer)
		{
			ActiveCmdBuffer
		}
	}

	VulkanCommandBuffer* VulkanCommandBufferManager::GetImmediateCommandBuffer()
	{
		return nullptr;
	}

	void VulkanCommandBufferManager::SubmitActiveCommandBuffer(WEngine::WArray<VulkanSemaphore*>& SignalSemaphores)
	{
		for (VulkanSemaphore* IMSemaphore : ImmediateDoneSemaphores)
		{
			ActiveCmdBuffer->AddWaitingSemaphore(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, IMSemaphore);
		}
		ImmediateDoneSemaphores.Clear();

		WEngine::WArray<VkSemaphore> Semaphores(SignalSemaphores.Size());
		for (VulkanSemaphore* pSemaphore : SignalSemaphores)
		{
			Semaphores.Push(pSemaphore->GetHandle());
		}
		Semaphores.Push(pActiveSemaphore->GetHandle());

		pQueue->Submit(ActiveCmdBuffer, Semaphores);
	}

	void VulkanCommandBufferManager::SubmitImmediateCommandBuffer(WEngine::WArray<VulkanSemaphore*>& SignalSemaphores)
	{
		WEngine::WArray<VkSemaphore> Semaphores(SignalSemaphores.Size());
		for (VulkanSemaphore* pSemaphore : SignalSemaphores)
		{
			Semaphores.Push(pSemaphore->GetHandle());
		}
		pQueue->Submit(ImmediateCmdBuffer, Semaphores);
	}

	void VulkanCommandBufferManager::WaitForCommandBuffer(VulkanCommandBuffer* CmdBuffer, double Time)
	{
		CmdBuffer->GetFence()->Wait(Time);
	}

}