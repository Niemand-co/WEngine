#pragma once
#include "RHI/Public/RHIPipelineStateObject.h"

namespace Vulkan
{

	class VulkanPipelineStateObject : public RHIPipelineStateObject
	{
	public:

		VulkanPipelineStateObject(VkPipeline *pipeline);

		virtual ~VulkanPipelineStateObject();

		VkPipeline* GetHandle();

	private:

		VkPipeline *m_pipeline;

	};

}