#pragma once
#include "RHI/Public/RHISampler.h"

class VulkanSampler : public RHISampler
{
public:

	VulkanSampler(VkSampler *pSampler);

	virtual ~VulkanSampler();

	VkSampler* GetHandle();

private:

	VkSampler *m_pSampler;

};