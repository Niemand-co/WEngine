#include "pch.h"
#include "Platform/Vulkan/Public/VulkanQueue.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanCommandPool.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Platform/Vulkan/Public/VulkanSwapchain.h"
#include "Platform/Vulkan/Public/VulkanSemaphore.h"
#include "Platform/Vulkan/Public/VulkanFence.h"
#include "Render/Descriptor/Public/RHIQueueDescriptor.h"
#include "Render/Descriptor/Public/RHISubmitDescriptor.h"
#include "Utils/Public/Window.h"

namespace Vulkan
{

	VulkanQueue::VulkanQueue(VulkanDevice *pInDevice, uint32 inQueueFamilyIndex)
		: pDevice(pInDevice), QueueFamilyIndex(inQueueFamilyIndex)
	{
		vkGetDeviceQueue(pDevice->GetHandle(), inQueueFamilyIndex, 0, &Queue);
	}

	VulkanQueue::~VulkanQueue()
	{
	}

	int VulkanQueue::GetIndex()
	{
		return QueueFamilyIndex;
	}

	RHICommandPool* VulkanQueue::GetCommandPool(VulkanCommandBufferManager *pInManager)
	{
		VkCommandPoolCreateInfo CommandPoolCreateInfo = {};
		CommandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		CommandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		CommandPoolCreateInfo.queueFamilyIndex = QueueFamilyIndex;

		return new VulkanCommandPool(pDevice, &CommandPoolCreateInfo, pInManager);
	}

	void VulkanQueue::Submit(VulkanCommandBuffer *CmdBuffer, uint32 NumSignalSemaphore, VkSemaphore *pSignalSemaphores)
	{
		VkCommandBuffer CommandBuffer = CmdBuffer->GetHandle();

		WEngine::WArray<VkSemaphore> WaitingSemaphores;
		WaitingSemaphores.Reserve(CmdBuffer->GetWaitingSemaphores().Size());
		for (const WEngine::WSharedPtr<VulkanSemaphore>& Semaphore : CmdBuffer->GetWaitingSemaphores())
		{
			WaitingSemaphores.Push(Semaphore->GetHandle());
		}
		VkSubmitInfo Info = {};
		{
			Info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			Info.commandBufferCount = 1;
			Info.pCommandBuffers = &CommandBuffer;
			Info.waitSemaphoreCount = WaitingSemaphores.Size();
			Info.pWaitSemaphores = WaitingSemaphores.GetData();
			Info.pWaitDstStageMask = CmdBuffer->GetWaitingStageMasks().GetData();
			Info.signalSemaphoreCount = NumSignalSemaphore;
			Info.pSignalSemaphores = pSignalSemaphores;
		}
		vkQueueSubmit(Queue, 1, &Info, CmdBuffer->GetFence()->GetHandle());
		LastSubmittedCmdBuffer = CmdBuffer;

		CmdBuffer->State = VulkanCommandBuffer::ECmdState::Submitted;
		CmdBuffer->WaitingStageMasks.Clear();
		CmdBuffer->SubmittedWaitingSemaphores = CmdBuffer->WaitingSemaphores;
		CmdBuffer->WaitingSemaphores.Clear();
	}

}