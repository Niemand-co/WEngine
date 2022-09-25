#include "pch.h"
#include "Platform/Vulkan/Public/VulkanPipelineStateObject.h"

namespace Vulkan
{

	VulkanPipelineStateObject::VulkanPipelineStateObject(VkPipeline* pipeline)
		: m_pipeline(pipeline)
	{
	}

	VulkanPipelineStateObject::~VulkanPipelineStateObject()
	{
	}

	VkPipeline* VulkanPipelineStateObject::GetHandle()
	{
		return m_pipeline;
	}

}