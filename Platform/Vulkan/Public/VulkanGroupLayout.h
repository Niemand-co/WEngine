#pragma once
#include "RHI/Public/RHIGroupLayout.h"

class VulkanGroupLayout : public RHIGroupLayout
{
public:

	VulkanGroupLayout(VkDescriptorSetLayout *pDescriptorSetLayout, unsigned int bindingCount);

	virtual ~VulkanGroupLayout();

	VkDescriptorSetLayout* GetHandle();

private:

	VkDescriptorSetLayout *m_pDescriptorSetLayout;

};