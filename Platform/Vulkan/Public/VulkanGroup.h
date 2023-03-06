#pragma once
#include "RHI/Public/RHIGroup.h"

class VulkanGroup : public RHIGroup
{
public:

	VulkanGroup(class VulkanDevice *pInDevice);

	virtual ~VulkanGroup();

	VkDescriptorSet GetHandle() const { return DescriptorSet; }

private:

	VulkanDevice *pDevice;

	VkDescriptorSet DescriptorSet;	

};