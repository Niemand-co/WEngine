#pragma once
#include "RHI/Public/RHIFence.h"

class VulkanFence : public RHIFence
{
public:

	VulkanFence(VkFence *fence);

	virtual ~VulkanFence();

private:

	VkFence *m_fence;

};