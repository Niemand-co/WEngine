#include "pch.h"
#include "Platform/Vulkan/Public/VulkanQueue.h"
#include "Platform/Vulkan/Public/VulkanCommandPool.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Platform/Vulkan/Public/VulkanSwapchain.h"
#include "Platform/Vulkan/Public/VulkanSemaphore.h"
#include "Platform/Vulkan/Public/VulkanFence.h"
#include "Render/Descriptor/Public/RHIQueueDescriptor.h"
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
		commandPoolCreateInfo.queueFamilyIndex = m_queueFamilyIndex;

		VkCommandPool *pool = (VkCommandPool*)Allocator::Allocate(sizeof(VkCommandPool));
		vkCreateCommandPool(*m_device, &commandPoolCreateInfo, nullptr, pool);

		return new VulkanCommandPool(pool, m_device);
	}

	void VulkanQueue::Submit(RHICommandBuffer **cmd, unsigned int count, RHISemaphore *waitSemaphore, RHISemaphore *signalSemaphore, RHIFence *fence)
	{
		std::vector<VkCommandBuffer> commandbuffers(count);
		for (unsigned int i = 0; i < count; ++i)
		{
			commandbuffers[i] = *static_cast<VulkanCommandBuffer*>(cmd[i])->GetHandle();
		}

		VkPipelineStageFlags pipelineStateFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = commandbuffers.size();
		submitInfo.pCommandBuffers = commandbuffers.data();
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = static_cast<VulkanSemaphore*>(waitSemaphore)->GetHandle();
		submitInfo.pWaitDstStageMask = pipelineStateFlags;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = static_cast<VulkanSemaphore*>(signalSemaphore)->GetHandle();

		if(fence != nullptr)
			vkQueueSubmit(*m_queue, 1, &submitInfo, *static_cast<VulkanFence*>(fence)->GetHandle());
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

}