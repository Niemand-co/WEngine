#include "pch.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIInstance.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHICommandPool.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "RHI/Public/RHIQueue.h"
#include "RHI/Public/RHISwapchain.h"
#include "RHI/Public/RHISemaphore.h"
#include "Render/Descriptor/Public/RHIInstanceDescriptor.h"

RHIContext::RHIContext(RHIQueue *queue, RHIDevice *device)
	: m_pQueue(queue), m_pDevice(device)
{
	m_pPool = queue->GetCommandPool();
}

void RHIContext::Init()
{
}

RHICommandBuffer* RHIContext::GetCommandBuffer()
{
	return m_pPool->GetCommandBuffer();
}

void RHIContext::ExecuteCommandBuffer(RHICommandBuffer* cmd)
{
	m_pCommandBuffers.push_back(cmd);
}

void RHIContext::Submit(RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore)
{
	m_pQueue->Submit(m_pCommandBuffers.data(), m_pCommandBuffers.size(), waitSemaphore, signalSemaphore);
}

void RHIContext::Present(unsigned int imageIndex, RHISwapchain *swapchain, RHISemaphore *semaphore)
{
	m_pQueue->Present(swapchain, imageIndex, semaphore);
}
