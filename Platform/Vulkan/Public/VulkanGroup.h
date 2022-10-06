#pragma once
#include "RHI/Public/RHIGroup.h"

class VulkanGroup : public RHIGroup
{
public:

	VulkanGroup(VkDescriptorSet *pDescriptorSet);

	virtual ~VulkanGroup();

	VkDescriptorSet* GetHandle();

private:

	VkDescriptorSet *m_pDescriptorSet;	

};