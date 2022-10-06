#pragma once
#include "RHI/Public/RHIPipelineResourceLayout.h"

class VulkanPipelineResourceLayout : public RHIPipelineResourceLayout
{
public:

	VulkanPipelineResourceLayout(VkPipelineLayout *pPipelineLayout);

	virtual ~VulkanPipelineResourceLayout();

	VkPipelineLayout* GetHandle();

private:

	VkPipelineLayout *m_pPipelineLayout;

};