#pragma once
#include "RHI/Public/RHIGroupPool.h"

class VulkanGroupPool : public RHIGroupPool
{
public:

	VulkanGroupPool(VkDescriptorPool *pDescriptorPool, RHIGroupLayout *pGroupLayout, VkDevice *pDevice);

	virtual ~VulkanGroupPool();

	virtual RHIGroup* GetGroup(unsigned int count = 1) override;

private:

	VkDescriptorPool *m_pDescriptorSetPool;

	VkDevice *m_pDevice;

};