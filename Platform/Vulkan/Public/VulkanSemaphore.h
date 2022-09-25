#pragma once
#include "RHI/Public/RHISemaphore.h"

class VulkanSemaphore : public RHISemaphore
{
public:

	VulkanSemaphore(VkSemaphore *semaphore);

	virtual ~VulkanSemaphore();

	VkSemaphore* GetHandle();

private:

	VkSemaphore *m_semaphore;

};