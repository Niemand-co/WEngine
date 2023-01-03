#include "pch.h"
#include "Platform/Vulkan/Public/VulkanQueue.h"
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

	VulkanQueue::VulkanQueue(VkQueue* queue, int queueFamilyIndex, VkDevice *device)
	{
		m_queue = queue;
		m_device = device;
		m_queueFamilyIndex = queueFamilyIndex;
	}

	VulkanQueue::~VulkanQueue()
	{
	}

	int VulkanQueue::GetIndex()
	{
		return m_queueFamilyIndex;
	}

	RHICommandPool* VulkanQueue::GetCommandPool()
	{
		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolCreateInfo.queueFamilyIndex = m_queueFamilyIndex;

		VkCommandPool *pPool = (VkCommandPool*)WEngine::Allocator::Get()->Allocate(sizeof(VkCommandPool));
		::new (pPool) VkCommandPool();
		RE_ASSERT(vkCreateCommandPool(*m_device, &commandPoolCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pPool) == VK_SUCCESS, "Failed to Get Command Pool.");

		RHICommandPool *pool = (RHICommandPool*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanCommandPool));
		::new (pool) VulkanCommandPool(pPool, m_device);

		return pool;
	}

	void VulkanQueue::Submit(RHISubmitDescriptor* descriptor)
	{
		WEngine::WArray<VkCommandBuffer> commandbuffers(descriptor->commandBufferCount);
		for (unsigned int i = 0; i < descriptor->commandBufferCount; ++i)
		{
			commandbuffers[i] = *static_cast<VulkanCommandBuffer*>(descriptor->pCommandBuffers[i])->GetHandle();
		}

		WEngine::WArray<VkSemaphore> waitSemaphores(descriptor->waitSemaphoreCount);
		for (unsigned int i = 0; i < descriptor->waitSemaphoreCount; ++i)
		{
			waitSemaphores[i] = *static_cast<VulkanSemaphore*>(descriptor->pWaitSemaphores[i])->GetHandle();
		}

		WEngine::WArray<VkSemaphore> signalSemaphores(descriptor->signalSemaphoreCount);
		for (unsigned int i = 0; i < descriptor->signalSemaphoreCount; ++i)
		{
			signalSemaphores[i] = *static_cast<VulkanSemaphore*>(descriptor->pSignalSemaphores[i])->GetHandle();
		}

		VkPipelineStageFlags pipelineStateFlags[] = { descriptor->waitStage };
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = commandbuffers.Size();
		submitInfo.pCommandBuffers = commandbuffers.GetData();
		submitInfo.waitSemaphoreCount = waitSemaphores.Size();
		submitInfo.pWaitSemaphores = waitSemaphores.GetData();
		submitInfo.pWaitDstStageMask = pipelineStateFlags;
		submitInfo.signalSemaphoreCount = signalSemaphores.Size();
		submitInfo.pSignalSemaphores = signalSemaphores.GetData();

		if(descriptor->pFence != nullptr)
			vkQueueSubmit(*m_queue, 1, &submitInfo, *static_cast<VulkanFence*>(descriptor->pFence)->GetHandle());
		else
			vkQueueSubmit(*m_queue, 1, &submitInfo, VK_NULL_HANDLE);
	}

	bool VulkanQueue::Present(RHISwapchain *swapchain, unsigned int index, RHISemaphore *semaphore)
	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = static_cast<VulkanSemaphore*>(semaphore)->GetHandle();
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = static_cast<VulkanSwapchain*>(swapchain)->GetHandle();
		presentInfo.pImageIndices = &index;
		presentInfo.pResults = nullptr;
		
		VkResult result = vkQueuePresentKHR(*m_queue, &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || Window::cur_window->IsSizeChanged())
		{
			return false;
		}
		else if (result != VK_SUCCESS)
		{
			RE_ASSERT(false, "Failed to Present Image.");
		}
		return true;
	}

	VkQueue* VulkanQueue::GetHandle()
	{
		return m_queue;
	}

}